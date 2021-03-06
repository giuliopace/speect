######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for Lexicon MaryTTS FST formatter plug-in                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/lexicon_maryttsfst.c
  src/serialized_lex.c
  src/read.c
  )


######## header files ##################

speect_plugin_headers(
  src/lexicon_maryttsfst.h
  src/serialized_lex.h
  )

