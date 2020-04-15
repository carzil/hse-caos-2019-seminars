#include <iostream>

#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <asm/unistd.h>
#include <signal.h>
#include <unordered_map>


class ProcessFinishedException : public std::exception {
public:
    virtual const char* what() const noexcept override {
       return "process finished";
    }
};


class SystemError : public std::exception {
public:
    explicit SystemError(std::string error_prefix) {
        msg_ = std::move(error_prefix);
        msg_ += ": ";
        msg_ += std::strerror(errno);
    }

    virtual const char* what() const noexcept override {
       return msg_.c_str();
    }

protected:
    std::string msg_;
};


constexpr long kBreakPointAddr = 0x401126;


class Breakpoint {
public:
    Breakpoint(std::string name, long orig_word) : name_(std::move(name)), orig_word_(orig_word) {
    }

    long GetOrigWord() const {
        return orig_word_;
    }

    const std::string& Name() const {
        return name_;
    }

private:
    std::string name_;
    long orig_word_ = 0;
};

long InjectInt3(long word) {
    return (word & 0xffffffffffffff00) | 0xcc;
}

class MiniGdb {
public:
    void Start(char* cmd) {
        pid_ = fork();
        if (pid_ < 0) {
            throw SystemError("fork");
        } else if (pid_ == 0) {
            ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
            execl(cmd, cmd, NULL);
            exit(1);
        }
        std::cerr << "started child " << pid_ << std::endl;
    }

private:
    using UserRegs = struct user_regs_struct;

    // Utility functions

    long ReadWord(long addr) {
        long ret = ptrace(PTRACE_PEEKDATA, pid_, addr, 0);
        if (ret < 0) {
            throw SystemError("ReadWord");
        }

        return ret;
    }

    long WriteWord(long addr, long word) {
        long ret = ptrace(PTRACE_POKEDATA, pid_, addr, word);
        if (ret < 0) {
            throw SystemError("WriteWord");
        }

        return ret;
    }

    void WaitStop() {
        int status = 0;
        int ret = waitpid(pid_, &status, 0);
        if (ret < 0) {
            throw SystemError("waitpid");
        }

        if (WIFEXITED(status)) {
            std::cerr << "process exited, status = " << WEXITSTATUS(status) << std::endl;
            throw ProcessFinishedException{};
        } else if (WIFSIGNALED(status)) {
            std::cerr << "process signaled, signal = " << WTERMSIG(status) << std::endl;
            throw ProcessFinishedException{};
        } else if (WIFSTOPPED(status)) {
            // std::cerr << "process stopped, signal = " << WSTOPSIG(status) << std::endl;
            return;
        }
    }

    UserRegs GetRegs() {
        UserRegs regs;
        long ret = ptrace(PTRACE_GETREGS, pid_, 0, &regs);
        if (ret < 0) {
            throw SystemError("GetRegs");
        }

        return regs;
    }

    void SetRegs(const UserRegs& regs) {
        long ret = ptrace(PTRACE_SETREGS, pid_, 0, &regs);
        if (ret < 0) {
            throw SystemError("SetRegs");
        }
    }

    void Continue() {
        long ret = ptrace(PTRACE_CONT, pid_, 0, 0);
        if (ret < 0) {
            throw SystemError("Continue");
        }
    }

    void SingleStep() {
        long ret = ptrace(PTRACE_SINGLESTEP, pid_, 0, 0);
        if (ret < 0) {
            throw SystemError("SingleStep");
        }
    }

    void DumpRegs() {
        UserRegs regs = GetRegs();
        std::cout << "r15 = " << std::hex << regs.r15 << std::endl;
        std::cout << "r14 = " << std::hex << regs.r14 << std::endl;
        std::cout << "r13 = " << std::hex << regs.r13 << std::endl;
        std::cout << "r12 = " << std::hex << regs.r12 << std::endl;
        std::cout << "rbp = " << std::hex << regs.rbp << std::endl;
        std::cout << "rbx = " << std::hex << regs.rbx << std::endl;
        std::cout << "r11 = " << std::hex << regs.r11 << std::endl;
        std::cout << "r10 = " << std::hex << regs.r10 << std::endl;
        std::cout << "r9 = " << std::hex << regs.r9 << std::endl;
        std::cout << "r8 = " << std::hex << regs.r8 << std::endl;
        std::cout << "rax = " << std::hex << regs.rax << std::endl;
        std::cout << "rcx = " << std::hex << regs.rcx << std::endl;
        std::cout << "rdx = " << std::hex << regs.rdx << std::endl;
        std::cout << "rsi = " << std::hex << regs.rsi << std::endl;
        std::cout << "rdi = " << std::hex << regs.rdi << std::endl;
        std::cout << "orig_rax = " << std::hex << regs.orig_rax << std::endl;
        std::cout << "rip = " << std::hex << regs.rip << std::endl;
        std::cout << "cs = " << std::hex << regs.cs << std::endl;
        std::cout << "eflags = " << std::hex << regs.eflags << std::endl;
        std::cout << "rsp = " << std::hex << regs.rsp << std::endl;
        std::cout << "ss = " << std::hex << regs.ss << std::endl;
        std::cout << "fs_base = " << std::hex << regs.fs_base << std::endl;
        std::cout << "gs_base = " << std::hex << regs.gs_base << std::endl;
        std::cout << "ds = " << std::hex << regs.ds << std::endl;
        std::cout << "es = " << std::hex << regs.es << std::endl;
        std::cout << "fs = " << std::hex << regs.fs << std::endl;
        std::cout << "gs = " << std::hex << regs.gs << std::endl;
    }

    void AddBreakpoint(std::string name, long addr) {
        long word = ReadWord(addr);
        breakpoints_.emplace(addr + 1, Breakpoint{std::move(name), word});
        WriteWord(addr, InjectInt3(word));
    }

    void ProcessBreakpoint(const Breakpoint& breakpoint, const UserRegs& regs) {
        std::cerr << "triggered breakpoint '" << breakpoint.Name() << "'" << std::endl;
        ProcessCmd();

        uint64_t breakpointAddr = regs.rip - 1;

        // restore original instruction
        long orig_word = breakpoint.GetOrigWord();
        WriteWord(breakpointAddr, orig_word);

        // move rip one instruction backward (int3 = 0xcc => 1 byte)
        UserRegs newRegs = regs;
        newRegs.rip -= 1;
        SetRegs(newRegs);

        // perform original instruction
        SingleStep();
        WaitStop();

        // write breakpoint back
        WriteWord(breakpointAddr, InjectInt3(orig_word));
    }

    void Iteration() {
        WaitStop();

        UserRegs regs = GetRegs();

        if (auto it = breakpoints_.find(regs.rip); it != breakpoints_.end()) {
            ProcessBreakpoint(it->second, regs);
        }

        Continue();
    }

    void ProcessCmd() {
        for (;;) {
            std::cerr << ">>> ";
            std::string cmd;
            std::cin >> cmd;
            if (cmd == "regs") {
                DumpRegs();
            } else if (cmd == "continue" || cmd == "c") {
                break;
            } else {
                std::cerr << "unknown command: " << cmd << std::endl;
            }
        }
    }

public:
    void Loop() {
        try {
            WaitStop();
            AddBreakpoint("output", kBreakPointAddr);
            Continue();

            for (;;) {
                Iteration();
            }
        } catch (const ProcessFinishedException&) {
        }
    }

private:
    pid_t pid_ = -1;
    std::unordered_map<long, Breakpoint> breakpoints_;
};


int main(int argc, char *argv[]) {
    MiniGdb mini_gdb;

    mini_gdb.Start(argv[1]);

    mini_gdb.Loop();

    return 0;
}
