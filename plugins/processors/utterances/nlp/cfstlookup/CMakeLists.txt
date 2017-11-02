######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 5 November 2009                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## CMakeList for cfstLookup utterance processor plug-in                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

add_subdirectory(cfstlookup)
find_package(cfstlookup REQUIRED)

#------------------------------------------------------------------------------------#
#                             Define plug-in                                         #
#------------------------------------------------------------------------------------#

speect_plugin_definition(cfstLookup_UttProc "ScfstlookupUttProc" 1 0 0)


#------------------------------------------------------------------------------------#
#                        Configure plugin_info.h.in                                  #
#------------------------------------------------------------------------------------#

set(description "An Utterance Processor class that does the lexical lookup (addendum, lexicon, g2p) of the words in the Word relation. Creates SylStructure, Syllable, and Phone relations")

# Minimum required Speect Engine version
set(major_min 1)
set(minor_min 0)

speect_plugin_configure_info(${description} ${major_min} ${minor_min})

speect_plugin_include_dirs(${CFSTLOOKUP_INCLUDE_DIRS})


#------------------------------------------------------------------------------------#
#                            Include lexical plug-ins                                #
#------------------------------------------------------------------------------------#

include(lexicon)
include(addendum)
include(g2p)
include(syllabification)
include(phoneset)

#------------------------------------------------------------------------------------#
#                               Source files                                         #
#------------------------------------------------------------------------------------#

# Lists of all the source files
include(sources)


#------------------------------------------------------------------------------------#
#                        Plug-in shared object                                       #
#------------------------------------------------------------------------------------#

speect_plugin_create()
