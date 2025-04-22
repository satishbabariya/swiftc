// filepath: /Users/satishbabariya/CLionProjects/swiftc/lib/Diagnostic/DiagnosticEngine.cpp
/**
 * @file DiagnosticEngine.cpp
 * @brief Implementation of the DiagnosticEngine class.
 *
 * This file contains the implementation of the DiagnosticEngine class, which
 * is responsible for managing and emitting diagnostics during compilation.
 */

#include "swift/Diagnostic/DiagnosticEngine.h"

namespace swift {

DiagnosticEngine::DiagnosticEngine(const SourceManager &SM) : SM(SM) {}

void DiagnosticEngine::addConsumer(std::unique_ptr<DiagnosticConsumer> Consumer) {
    Consumers.push_back(std::move(Consumer));
}

void DiagnosticEngine::error(SourceLocation Loc, const std::string &Message) {
    Diagnostic Diag(DiagnosticSeverity::Error, Loc, Message);
    emitDiagnostic(Diag);
    ++NumErrors;
}

void DiagnosticEngine::warning(SourceLocation Loc, const std::string &Message) {
    Diagnostic Diag(DiagnosticSeverity::Warning, Loc, Message);
    emitDiagnostic(Diag);
    ++NumWarnings;
}

void DiagnosticEngine::note(SourceLocation Loc, const std::string &Message) {
    Diagnostic Diag(DiagnosticSeverity::Note, Loc, Message);
    emitDiagnostic(Diag);
    ++NumNotes;
}

void DiagnosticEngine::remark(SourceLocation Loc, const std::string &Message) {
    Diagnostic Diag(DiagnosticSeverity::Remark, Loc, Message);
    emitDiagnostic(Diag);
    ++NumRemarks;
}

bool DiagnosticEngine::hasErrors() const {
    return NumErrors > 0;
}

unsigned DiagnosticEngine::getTotalDiagnosticCount() const {
    return NumErrors + NumWarnings + NumNotes + NumRemarks;
}

unsigned DiagnosticEngine::getErrorCount() const {
    return NumErrors;
}

unsigned DiagnosticEngine::getWarningCount() const {
    return NumWarnings;
}

void DiagnosticEngine::emitDiagnostic(const Diagnostic &Diag) {
    for (const auto &Consumer : Consumers) {
        Consumer->handleDiagnostic(Diag, SM);
    }
}

} // namespace swift