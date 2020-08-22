;// ***** MZCSvcMessages.mc *****

;// See http://msdn.microsoft.com/en-us/library/windows/desktop/dd996906(v=vs.85).aspx

;// Header section

MessageIdTypedef=DWORD

SeverityNames = (
    Success=0x0:STATUS_SEVERITY_SUCCESS
    Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
    Warning=0x2:STATUS_SEVERITY_WARNING
    Error=0x3:STATUS_SEVERITY_ERROR
)

FacilityNames = (
    System=0x0:FACILITY_SYSTEM
    Runtime=0x2:FACILITY_RUNTIME
    Stubs=0x3:FACILITY_STUBS
    Io=0x4:FACILITY_IO_ERROR_CODE
)

LanguageNames = (English=0x409:MSG00409)
LanguageNames = (Italian=0x410:MSG00410)

;// Category definitions.
MessageId=1
SymbolicName=CAT_APP
Language=English
App
.

;#define  CATEGORY_COUNT  1

;// Message definitions.

MessageId=0xE001
SymbolicName=MSG_SUCCESS
Severity=Success
Facility=Runtime
Language=English
%1
.

MessageId=+1
SymbolicName=MSG_INFORMATIONAL
Severity=Informational
Facility=Runtime
Language=English
%1
.

MessageId=+1
SymbolicName=MSG_WARNING
Severity=Warning
Facility=Runtime
Language=English
%1
.

MessageId=+1
SymbolicName=MSG_ERROR
Severity=Error
Facility=Runtime
Language=English
%1
.


