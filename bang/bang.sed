::::::::::::::
bang
::::::::::::::
# usage:  cat prog.APL | bang  (translate APL2 source to "APL bang")
# refer:  APL!  A simple, intuitive, and terse rendition of APL as ASCII
# author: (Bill) William I. Chang (wchang@cshl.org) 516-367-8866 Winter 1994
# notes:  put APL comment symbol in bang; APL delta symbol in quote.sed;
#         and other APL symbols in ascii.sed; uppercase keywords also available
# test:   has been tested on Richard Levine's Toronto Toolkit 2.1 (May 1992)
#
# first find unquoted comment symbols (‰), then parse into separate phrases:
# ascii comment/apl comment/ascii string literal/apl string literal/apl code
# before translation and cleaning up.
#
sed "s/'\([^']*\)'/ }\\
'\1' }\\
/g" |\
sed "/^[^']/s/‰/‰{/" |\
sed -f joinqq.sed |\
sed "s/{/ }\\
}/" |\
sed -f split.sed |\
sed -f quote.sed |\
sed -f ascii.sed |\
sed -f trim.sed |\
sed -f join.sed
::::::::::::::
joinqq.sed
::::::::::::::
# join lines only at internal quote marks, i.e. '...''...' and change '' to ``
:joinqq
/ }$/{N
s/' }\n'/``/
s/ }\n//
b joinqq
}
::::::::::::::
split.sed
::::::::::::::
# special case to include the delta character as "ASCII" &; escape original &
s/&/`\&`/g
s/∂/\&/g
# protect comments that are ASCII plus delta (&) with "leading-blanks"comment"
/^}[[:print:]][[:print:]]*$/{
s/^}\( *\)/"\1"/
s/$/"/
}
# split other comments at quotemarks, insert } before stuff outside quotes
/^}/{
s/'\([^']*\)'/ }\
'\1' }\
}/g
}
# split other lines at quotemarks; continuation is marked with blank}
/^[^"}]/{
s/'\([^']*\)'/ }\
'\1' }\
/g
}
::::::::::::::
quote.sed
::::::::::::::
# change `` back to ''
/^[^"]/s/``/''/g
# string literals that are entirely ASCII plus delta (&) are switched to "..."
/^'[[:print:]]*$/{
s/^'/"/
s/'$/"/
s/' }$/" }/
}
# those that are not entirely ASCII plus delta (&) are marked with }
#/^'.*[^[:print:]]/s/^'/}/
# delete empty lines introduced by splitting a comment at quotemarks
/^} }$/d
# if an identifier in active code conflicts with a keyword, backquote it
/^[^'"{}]/{
s/\([^a-zA-Z0-9_&]\)not\([^a-zA-Z0-9_&]\)/\1`not`\2/g
s/\([^a-zA-Z0-9_&]\)and\([^a-zA-Z0-9_&]\)/\1`and`\2/g
s/\([^a-zA-Z0-9_&]\)or\([^a-zA-Z0-9_&]\)/\1`or`\2/g
s/\([^a-zA-Z0-9_&]\)nand\([^a-zA-Z0-9_&]\)/\1`nand`\2/g
s/\([^a-zA-Z0-9_&]\)nor\([^a-zA-Z0-9_&]\)/\1`nor`\2/g
s/\([^a-zA-Z0-9_&]\)in\([^a-zA-Z0-9_&]\)/\1`in`\2/g
s/\([^a-zA-Z0-9_&]\)exp\([^a-zA-Z0-9_&]\)/\1`exp`\2/g
s/\([^a-zA-Z0-9_&]\)log\([^a-zA-Z0-9_&]\)/\1`log`\2/g
s/\([^a-zA-Z0-9_&]\)max\([^a-zA-Z0-9_&]\)/\1`max`\2/g
s/\([^a-zA-Z0-9_&]\)min\([^a-zA-Z0-9_&]\)/\1`min`\2/g
s/\([^a-zA-Z0-9_&]\)up\([^a-zA-Z0-9_&]\)/\1`up`\2/g
s/\([^a-zA-Z0-9_&]\)down\([^a-zA-Z0-9_&]\)/\1`down`\2/g
s/\([^a-zA-Z0-9_&]\)encode\([^a-zA-Z0-9_&]\)/\1`encode`\2/g
s/\([^a-zA-Z0-9_&]\)decode\([^a-zA-Z0-9_&]\)/\1`decode`\2/g
s/\([^a-zA-Z0-9_&]\)rotate\([^a-zA-Z0-9_&]\)/\1`rotate`\2/g
s/\([^a-zA-Z0-9_&]\)transpose\([^a-zA-Z0-9_&]\)/\1`transpose`\2/g
s/\([^a-zA-Z0-9_&]\)circle\([^a-zA-Z0-9_&]\)/\1`circle`\2/g
s/\([^a-zA-Z0-9_&]\)deal\([^a-zA-Z0-9_&]\)/\1`deal`\2/g
s/\([^a-zA-Z0-9_&]\)choose\([^a-zA-Z0-9_&]\)/\1`choose`\2/g
s/\([^a-zA-Z0-9_&]\)solve\([^a-zA-Z0-9_&]\)/\1`solve`\2/g
s/\([^a-zA-Z0-9_&]\)outer\([^a-zA-Z0-9_&]\)/\1`outer`\2/g
}
::::::::::::::
ascii.sed
::::::::::::::
# initial " means no translation needed
# initial ' or } means in '...' or comments, translation needed
# if neither then active code, translation needed
# protect funny ASCII combinations in a comment or string
/^['}]/{
s/o} /"o} "/g
s/<-/"<-"/g
s/->/"->"/g
s/<=/"<="/g
s/>=/">="/g
s/<>/"<>"/g
s/!/"!"/g
s/\?/"?"/g
s/\*/"*"/g
s/_/"_"/g
s/%/"%"/g
s/@/"@"/g
s/\$/"$"/g
s/#/"#"/g
#s/&/"\&"/g
#s/\^/"^"/g
#s/\~/"\~"/g
#s/`/"`"/g
s/\^/`and`/g
s/\~/`not`/g
}
# active code; eventually { becomes backquote; special case for (~ etc.
/^[^'"}]/{
s/<-/< -/g
s/->/- >/g
s/<=/< =/g
s/>=/> =/g
s/<>/< >/g
s/\!/{choose{/g
s/\?/{deal{/g
s/\^/{and{/g
s/\*/{exp{/g
s/\([]a-zA-Z0-9_&.)'" ]\)\~/\1{not{/g
}
# anywhere translation is needed
/^[^"]/{
s/‰/o} /g
s/Ù/\~=/g
s/Ω/<- /g
s/∏/->/g
s/Ú/<=/g
s/Û/>=/g
s/ÿ/<>/g
s/Ï/\!/g
s/Ê/\?/g
s/∆/\^/g
s/«/\~/g
s/ı/\*/g
s/∑/``/g
s/˝/`/g
#s/∂/\&/g
s/˜/_/g
s/ˆ/%/g
s/Î/{or{/g
s/Â/{nand{/g
s/Á/{nor{/g
s/Ó/{in{/g
s/ù/{decode{/g
s/ò/{encode{/g
s/µ/{log{/g
s/Ë/{rotate{/g
s/Ì/{transpose{/g
s/È/{rotate[@IO]{/g
s/Ò/\\[@IO]/g
s//\/[@IO]/g
s/ê/@/g
s/ë/`@/g
s/í/{solve{/g
s/Í/{circle{/g
s/¯/{outer{/g
s/˚/{up{/g
s/¸/{down{/g
s/Æ/$/g
s/Ø/#/g
s/©/{max{/g
s/æ/{min{/g
}
::::::::::::::
trim.sed
::::::::::::::
# in code, the { becomes a blank; strip some (not all) blanks help readability
/^[^'"}]/{
s/\([a-z]\){\([\/\\\.]\)/\1\2/g
s/\([^])'"@_\&a-zA-Z0-9-]\){\([a-z]\)/\1\2/g
s/\([^<]\)-{\([a-z]\)/\1\-\2/g
s/{{/{/g
s/{/ /g
}
# any remaining { were inside string literals; change to backquote
s/{/`/g
# strip leading } left by splitting comments
s/^}//
# strip leading " in ASCII comments
/^" *".*"$/s/^"//
# special cases involving quotes
s/""/''/g
s/" "/' '/g
s/"''"/''''/g
::::::::::::::
join.sed
::::::::::::::
# join lines that end with continuation marker blank}
:join
/ }$/{N
s/ }\n//
b join
}
