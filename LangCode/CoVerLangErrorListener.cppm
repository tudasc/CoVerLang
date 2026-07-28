module;

#include "antlr4-runtime.h"

export module CoVerLangErrorListener;

export class CoVerLangSyntaxError : public std::exception {
    public:
        CoVerLangSyntaxError(size_t _line, size_t _charpos, std::string _reason="") : linePosition(_line), charPositionInLine(_charpos), reason(_reason) {}
        const char * what() const noexcept override {
            return reason.empty() ? "Syntax error occured while parsing contract" : reason.c_str();
        }
        size_t linePos() { return linePosition; }
        size_t charPos() { return charPositionInLine; }
    private:
        antlr4::Token *offendingSymbol;
        const std::string reason;
        size_t linePosition;
        size_t charPositionInLine;
};

export class CoVerLangErrorListener : public antlr4::BaseErrorListener {
public:
    virtual void syntaxError(antlr4::Recognizer *recognizer,
                           antlr4::Token *offendingSymbol, size_t line,
                           size_t charPositionInLine, const std::string &msg,
                           std::exception_ptr e) override {
        // Don't perform any output here. That's for the pass to do if necessary.
        throw CoVerLangSyntaxError(line, charPositionInLine);
    };
};
