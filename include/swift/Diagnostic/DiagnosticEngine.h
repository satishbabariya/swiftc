/**
 * @file DiagnosticEngine.h
 * @brief Defines the DiagnosticEngine class which handles compilation diagnostics.
 *
 * This file contains the definition for DiagnosticEngine which is responsible for
 * managing and emitting diagnostics (errors, warnings, notes) during compilation.
 */

#ifndef SWIFT_DIAGNOSTIC_DIAGNOSTICENGINE_H
#define SWIFT_DIAGNOSTIC_DIAGNOSTICENGINE_H

#include "swift/Source/SourceLocation.h"
#include "swift/Source/SourceManager.h"

#include <string>
#include <vector>

namespace swift {
    /**
     * @enum DiagnosticSeverity
     * @brief Represents the severity level of a diagnostic.
     */
    enum class DiagnosticSeverity {
        Error,    ///< An error that prevents compilation from succeeding
        Warning,  ///< A warning about potentially problematic code
        Note,     ///< Additional information about a previous diagnostic
        Remark    ///< Informational remark not indicating a problem
    };

    /**
     * @class Diagnostic
     * @brief Represents a single diagnostic message.
     */
    class Diagnostic {
    public:
        /// The severity level of this diagnostic
        DiagnosticSeverity Severity;
        
        /// Source location where the diagnostic occurred
        SourceLocation Location;
        
        /// The diagnostic message
        std::string Message;

        /**
         * @brief Constructs a diagnostic with the specified parameters.
         * @param Severity The severity level
         * @param Location Source location of the issue
         * @param Message The diagnostic message text
         */
        Diagnostic(DiagnosticSeverity Severity, SourceLocation Location, std::string Message)
            : Severity(Severity), Location(Location), Message(std::move(Message)) {}
    };

    /**
     * @class DiagnosticConsumer
     * @brief Abstract base class for diagnostic consumers.
     *
     * Diagnostic consumers receive diagnostics from the DiagnosticEngine
     * and handle them in an implementation-specific way (e.g., print to console,
     * write to a file, etc.).
     */
    class DiagnosticConsumer {
    public:
        virtual ~DiagnosticConsumer() = default;

        /**
         * @brief Handles a diagnostic.
         * @param Diag The diagnostic to handle
         * @param SM The source manager for location information
         */
        virtual void handleDiagnostic(const Diagnostic &Diag, const SourceManager &SM) = 0;
    };

    /**
     * @class DiagnosticEngine
     * @brief Central manager for diagnostics in the Swift compiler.
     *
     * DiagnosticEngine is responsible for:
     * - Collecting diagnostics
     * - Formatting diagnostic messages
     * - Routing diagnostics to consumers
     * - Tracking diagnostics statistics
     */
    class DiagnosticEngine {
    public:
        /**
         * @brief Constructs a DiagnosticEngine with the given source manager.
         * @param SM The source manager to use for location information
         */
        explicit DiagnosticEngine(const SourceManager &SM);

        /**
         * @brief Adds a diagnostic consumer.
         * @param Consumer The consumer to add (ownership is transferred)
         */
        void addConsumer(std::unique_ptr<DiagnosticConsumer> Consumer);

        /**
         * @brief Emits an error diagnostic.
         * @param Loc The source location of the error
         * @param Message The error message
         */
        void error(SourceLocation Loc, const std::string &Message);

        /**
         * @brief Emits a warning diagnostic.
         * @param Loc The source location of the warning
         * @param Message The warning message
         */
        void warning(SourceLocation Loc, const std::string &Message);

        /**
         * @brief Emits a note diagnostic.
         * @param Loc The source location for the note
         * @param Message The note message
         */
        void note(SourceLocation Loc, const std::string &Message);

        /**
         * @brief Emits a remark diagnostic.
         * @param Loc The source location for the remark
         * @param Message The remark message
         */
        void remark(SourceLocation Loc, const std::string &Message);

        /**
         * @brief Returns whether any errors have been reported.
         * @return True if any errors have been reported
         */
        [[nodiscard]] bool hasErrors() const;

        /**
         * @brief Returns the total number of diagnostics emitted.
         * @return Total number of diagnostics
         */
        [[nodiscard]] unsigned getTotalDiagnosticCount() const;

        /**
         * @brief Returns the number of errors emitted.
         * @return Number of errors
         */
        [[nodiscard]] unsigned getErrorCount() const;

        /**
         * @brief Returns the number of warnings emitted.
         * @return Number of warnings
         */
        [[nodiscard]] unsigned getWarningCount() const;

    private:
        /// The source manager used for location information
        const SourceManager &SM;
        
        /// The list of diagnostic consumers
        std::vector<std::unique_ptr<DiagnosticConsumer>> Consumers;
        
        /// Count of error diagnostics
        unsigned NumErrors = 0;
        
        /// Count of warning diagnostics
        unsigned NumWarnings = 0;
        
        /// Count of note diagnostics
        unsigned NumNotes = 0;
        
        /// Count of remark diagnostics
        unsigned NumRemarks = 0;

        /**
         * @brief Emits a diagnostic to all consumers.
         * @param Diag The diagnostic to emit
         */
        void emitDiagnostic(const Diagnostic &Diag);
    };
} // namespace swift

#endif // SWIFT_DIAGNOSTIC_DIAGNOSTICENGINE_H