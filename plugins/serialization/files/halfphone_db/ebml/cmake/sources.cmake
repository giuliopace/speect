######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Source files for HalfphoneDB EBML formatter plug-in                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


######## source files ##################

speect_plugin_sources(
  src/plugin.c
  src/halfphone_db_ebml.c
  src/serialized_halfphone_db.c
  src/read.c
  )
 

######## header files ##################

speect_plugin_headers(
  src/halfphone_db_ebml.h
  src/serialized_halfphone_db.h
  src/element_id.h
  )

