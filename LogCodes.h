//---------------------------------------------------------------------------

#ifndef LogCodesH
#define LogCodesH

// Formatters and string constants
#define  LOG_DATE_TIME_FORMAT        1000
#define  LOG_ITEM_FMT_TXT            1001
#define  LOG_ITEM_SYSLOG_FMT_TXT     1002
#define  LOG_ITEM_HDR_TXT            1003
#define  LOG_ITEM_HDR_DT_TXT         1004
#define  LOG_ITEM_SEP_TXT            1005
#define  LOG_FILE_CREATION_MSG       1006

// Severities
#define  LOG_SEVERITY_BASE           1010
#define  LOG_SEV_EMERGENCY           1010
#define  LOG_SEV_ALERT               1011
#define  LOG_SEV_CRITICAL            1012
#define  LOG_SEV_ERROR               1013
#define  LOG_SEV_WARNING             1014
#define  LOG_SEV_NOTICE              1015
#define  LOG_SEV_INFORMATIONAL       1016
#define  LOG_SEV_DEBUG               1017

#define  LOG_SEVERITY_BASE_SHORT      1020
#define  LOG_SEV_SHORT_EMERGENCY      1020
#define  LOG_SEV_SHORT_ALERT          1021
#define  LOG_SEV_SHORT_CRITICAL       1022
#define  LOG_SEV_SHORT_ERROR          1023
#define  LOG_SEV_SHORT_WARNING        1024
#define  LOG_SEV_SHORT_NOTICE         1025
#define  LOG_SEV_SHORT_INFORMATIONAL  1026
#define  LOG_SEV_SHORT_DEBUG          1027

// Facilities
#define  LOG_FACILITY_BASE            1100
#define  LOG_FAC_KERNEL               1100
#define  LOG_FAC_USERLEVEL            1101
#define  LOG_FAC_MAILSYSTEM           1102
#define  LOG_FAC_SYSTEMDAEMON         1103
#define  LOG_FAC_SECURITYONE          1104
#define  LOG_FAC_SYSLOGINTERNAL       1105
#define  LOG_FAC_LPR                  1106
#define  LOG_FAC_NNTP                 1107
#define  LOG_FAC_UUCP                 1108
#define  LOG_FAC_CLOCKDAEMONONE       1109
#define  LOG_FAC_SECURITYTWO          1110
#define  LOG_FAC_FTPDAEMON            1111
#define  LOG_FAC_NTP                  1112
#define  LOG_FAC_LOGAUDIT             1113
#define  LOG_FAC_LOGALERT             1114
#define  LOG_FAC_CLOCKDAEMONTWO       1115
#define  LOG_FAC_LOCALUSE0            1116
#define  LOG_FAC_LOCALUSE1            1117
#define  LOG_FAC_LOCALUSE2            1118
#define  LOG_FAC_LOCALUSE3            1119
#define  LOG_FAC_LOCALUSE4            1120
#define  LOG_FAC_LOCALUSE5            1121
#define  LOG_FAC_LOCALUSE6            1122
#define  LOG_FAC_LOCALUSE7            1123

#define  LOG_FACILITY_BASE_SHORT          1150
#define  LOG_FAC_SHORT_KERNEL             1150
#define  LOG_FAC_SHORT_USERLEVEL          1151
#define  LOG_FAC_SHORT_MAILSYSTEM         1152
#define  LOG_FAC_SHORT_SYSTEMDAEMON       1153
#define  LOG_FAC_SHORT_SECURITYONE        1154
#define  LOG_FAC_SHORT_SYSLOGINTERNAL     1155
#define  LOG_FAC_SHORT_LPR                1156
#define  LOG_FAC_SHORT_NNTP               1157
#define  LOG_FAC_SHORT_UUCP               1158
#define  LOG_FAC_SHORT_CLOCKDAEMONONE     1159
#define  LOG_FAC_SHORT_SECURITYTWO        1160
#define  LOG_FAC_SHORT_FTPDAEMON          1161
#define  LOG_FAC_SHORT_NTP                1162
#define  LOG_FAC_SHORT_LOGAUDIT           1163
#define  LOG_FAC_SHORT_LOGALERT           1164
#define  LOG_FAC_SHORT_CLOCKDAEMONTWO     1165
#define  LOG_FAC_SHORT_LOCALUSE0          1166
#define  LOG_FAC_SHORT_LOCALUSE1          1167
#define  LOG_FAC_SHORT_LOCALUSE2          1168
#define  LOG_FAC_SHORT_LOCALUSE3          1169
#define  LOG_FAC_SHORT_LOCALUSE4          1170
#define  LOG_FAC_SHORT_LOCALUSE5          1171
#define  LOG_FAC_SHORT_LOCALUSE6          1172
#define  LOG_FAC_SHORT_LOCALUSE7          1173

// Entities
#define  LOG_ENTITY_BASE                  1200
#define  LOG_ENT_APPLICATION              1200

// Messages

#define  LOG_MSG_BASE                               2000

#define  LOG_MSG_FAILURE               LOG_MSG_BASE + 0
#define  LOG_MSG_UNKNOWN_FAILURE       LOG_MSG_BASE + 1
#define  LOG_MSG_DBG                   LOG_MSG_BASE + 2
#define  LOG_MSG_START_APP             LOG_MSG_BASE + 3
#define  LOG_MSG_END_APP               LOG_MSG_BASE + 4
#define  LOG_MSG_START_PROCESS_FOLDER  LOG_MSG_BASE + 5
#define  LOG_MSG_STOP_PROCESS_FOLDER   LOG_MSG_BASE + 6

#endif
