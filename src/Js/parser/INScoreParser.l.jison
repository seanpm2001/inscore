
/* INScore lexical grammar */

%{
function debugmsg(str){
//	typeof console !== 'undefined' ? console.log("  lex: " + str) : print("  lex: " + str);
}

// ----------------------------------------------------------------------------------------
// strip escape char '\' from escaped characters
function unescape (str) {
	var n = str.length;
	var outStr = "";
	// browse the string:  'r' is an index to the current read location
	//                     'w' is an index to the current write location
	for (r=0, w=0; r < n; r++) {		
		if (str[r] == '\\') {			// escape char '\' is found
			var next = str[r+1];
			if ((next != '\'') && (next != '"')) {	// escape only ' and "
				outStr += str[r];		// output the escape char 
			}
		}
		else outStr += str[r];			// no escape: put the current char at write location 
	}
 	return outStr;
}
%}

%lex

%options flex
%x DQSTR
%x QSTR
%x COMMENT
%x JSECTION
%x EXPRSECTION
%x OSCSECTION


DIGIT	[0-9]

STARTID [_a-zA-Z]		/* allowed char for identifier first char */
IDCHAR  [_a-zA-Z0-9]	/* allowed char for identifier next chars */

HOSTCHAR [-a-zA-Z0-9]	/* allowed character set for host names see RFC952 and RFC1123 */
FILECHAR [^ \t\\/?:*><|"'!#;=] 	/* allowed character for filename and path as defined for windows 7 with some specification for INScore compatibility: no = ; */

RECHAR  [-_a-zA-Z0-9?+*]  	/* regular expression characters as defined by the OSC specification */
RECLASS [-_a-zA-Z0-9^]		/* regular expression characters for class def */

SPACE	[ \t]
EOL		[\x0a\x0d]


%%


{DIGIT}+					return 'UINT';
"+"{DIGIT}+					return 'UINT';
"-"{DIGIT}+					return 'INT';

[+-]?{DIGIT}+"."{DIGIT}* 				return 'FLOAT';
[+-]?{DIGIT}+"."{DIGIT}+e[-+]?{DIGIT}+ 	return 'FLOAT';
[+-]?{DIGIT}+e[-+]?{DIGIT}+ 			return 'FLOAT';


"__END__"			debugmsg("ENDSCRIPT : " + yytext); return 'ENDSCRIPT';

// ----------------------------------------- 
//	identifiers 
// ----------------------------------------- 
{STARTID}{IDCHAR}*							debugmsg("IDENTIFIER: " + yytext); return 'IDENTIFIER';

// ----------------------------------------- 
//	hostname and IP 
// ----------------------------------------- 
({HOSTCHAR}({HOSTCHAR}*|'.'))+					debugmsg("HOSTNAME: " + yytext); return 'HOSTNAME';
{DIGIT}+"."{DIGIT}+"."{DIGIT}+"."{DIGIT}+		debugmsg("IPNUM: " + yytext); return 'IPNUM';

// ----------------------------------------- 
//	method requiring messages as argument 
// ----------------------------------------- 
"eval"				return 'EVAL';

// ----------------------------------------- 
//	quoted strings 
// ----------------------------------------- 
"\""               	this.begin('DQSTR');
<DQSTR>(\\\"|[^"])* { debugmsg ("STRING: " + unescape(yytext)); return 'STRING'; }
<DQSTR>"\""         this.popState();

"'"                 this.begin('QSTR');
<QSTR>(\\\'|[^'])*  { debugmsg ("STRING: " + unescape(yytext)); return 'STRING'; }
<QSTR>"'"           this.popState();

// ----------------------------------------- 
//	non quoted file path (without space)
// ----------------------------------------- 
("/"|(".""."?"/")*)({FILECHAR}+"/"?)+"."[a-zA-Z]+	 { debugmsg ("FILE STRING: " + yytext);	return 'STRING'; }

// ----------------------------------------- 
//	lang sections 
// ----------------------------------------- 
"<?"{SPACE}*"javascript"	this.begin('JSECTION');
<JSECTION>"?>"				this.popState();
<JSECTION>({EOL}|"?"[^>]|[^?])*	{ debugmsg ("JAVASCRIPT: " + yytext); return 'JSCRIPT'; }

// ----------------------------------------- 
//	regular expressions 
// ----------------------------------------- 
{RECHAR}+								{ debugmsg ("REGEXP: " + yytext); return 'REGEXP'; }
{RECHAR}*"["{RECLASS}+"]"{RECHAR}*		{ debugmsg ("REGEXP: " + yytext); return 'REGEXP'; }
{RECHAR}*"{"[_a-zA-Z0-9,]+"}"{RECHAR}*	{ debugmsg ("REGEXP: " + yytext); return 'REGEXP'; }

// ----------------------------------------- 
//	evaluable expression section 
// ----------------------------------------- 
<EXPRSECTION>"("		{ this.more(); this.begin('EXPRSECTION'); }
<EXPRSECTION>")"		{ this.more(); this.popState(); 
							if (this.topState() == 'INITIAL') {
							debugmsg("EXPRESSION " + yytext);
							return 'EXPRESSION'; 
							}
						}
<EXPRSECTION>[^()]*		{ this.more(); }
"expr"{SPACE}*"("		{ this.begin('EXPRSECTION'); this.more(); }

// ----------------------------------------- 
//	OSC address 
// ----------------------------------------- 
'/'										{ this.more(); this.begin('OSCSECTION'); }
<OSCSECTION>"/"							{ this.more(); }
<OSCSECTION>"$"?{STARTID}{IDCHAR}*			{ this.more(); }
<OSCSECTION>{RECHAR}+								{ this.more(); }
<OSCSECTION>{RECHAR}*"["{RECLASS}+"]"{RECHAR}*		{ this.more(); }
<OSCSECTION>{RECHAR}*"{"[_a-zA-Z0-9,]+"}"{RECHAR}*	{ this.more(); }
<OSCSECTION>{SPACE}|{EOL}				{ debugmsg("OSCADDRESS " + yytext); this.popState(); yytext = yytext.substring(0, yytext.length-1); return 'OSCADDRESS'; }

// ----------------------------------------- 
//	basic delimiters and markers 
// ----------------------------------------- 
"="					return 'EQUAL';
";"					return 'ENDEXPR';			/* end of expression */
"$"					return 'VARSTART';
":"					return 'COLON';
","					return 'COMMA';
"."					return 'POINT';
"("					return 'LEFTPAR';
")"					return 'RIGHTPAR';

// ----------------------------------------- 
//	comments 
// ----------------------------------------- 
'#'.*		debugmsg ("COMMENTLINE: " + yytext); 
'!'.*		debugmsg ("COMMENTLINE: " + yytext); 

<COMMENT>.|{EOL}* 	;
<COMMENT>"(*"		{ debugmsg ("BEGIN NESTED COMMENT"); this.begin('COMMENT'); } 
<COMMENT>"*)"       { debugmsg ("END COMMENT"); this.popState(); }
"(*"                { debugmsg ("BEGIN COMMENT"); this.begin('COMMENT'); }

{SPACE}+			;   /* eat up space */
{EOL}				;	/* yylloc->first_column=0; ignore */

<<EOF>>				return 'ENDSCRIPT';

.					{ debugmsg("ERR: " + yytext); return 'ERR'; }

