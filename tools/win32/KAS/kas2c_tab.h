typedef union {
    char    *string;    /* string buffer */
    int     number;     /* numeric value */
} YYSTYPE;
#define	QSTRING	258
#define	ID	259
#define	COMMENT2	260
#define	FSM	261
#define	ENDF	262
#define	STATES	263
#define	WATCH	264
#define	ENDW	265
#define	STATE	266
#define	ENDS	267
#define	INITIALIZE	268
#define	ENDI	269
#define	IF	270
#define	ELSEIF	271
#define	ELSE	272
#define	ENDIF	273
#define	IFONCE	274
#define	ENDIFONCE	275
#define	NUMBER	276
#define	AND	277
#define	OR	278
#define	NOT	279
#define	TEAM	280
#define	SHIPS	281
#define	TEAMSHIPS	282
#define	SHIPSPOINT	283
#define	TEAMSPOINT	284
#define	VOLUMEPOINT	285
#define	PATH	286
#define	POINT	287
#define	VOLUME	288
#define	THISTEAM	289
#define	THISTEAMSHIPS	290
#define	THISTEAMSPOINT	291
#define	JUMP	292
#define	TRUE	293
#define	FALSE	294
#define	LT	295
#define	LTE	296
#define	EQ	297
#define	NEQ	298
#define	GTE	299
#define	GT	300
#define	FSMCREATE	301
#define	LOCALIZATION	302
#define	ENDL	303
#define	LSTRING	304
#define	BANG	305
#define	UMINUS	306


extern YYSTYPE yylval;
