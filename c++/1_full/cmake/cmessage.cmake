
# https://stackoverflow.com/questions/18968979/how-to-make-colorized-message-with-cmake


# define colors
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColorReset   "${Esc}[m")
    set(ColorBold    "${Esc}[1m")
    set(Red          "${Esc}[31m")
    set(Green        "${Esc}[32m")
    set(Yellow       "${Esc}[33m")
    set(Blue         "${Esc}[34m")
    set(Magenta      "${Esc}[35m")
    set(Cyan         "${Esc}[36m")
    set(White        "${Esc}[37m")
    set(BoldRed      "${Esc}[1;31m")
    set(BoldGreen    "${Esc}[1;32m")
    set(BoldYellow   "${Esc}[1;33m")
    set(BoldBlue     "${Esc}[1;34m")
    set(BoldMagenta  "${Esc}[1;35m")
    set(BoldCyan     "${Esc}[1;36m")
    set(BoldWhite    "${Esc}[1;37m")
endif()


function(cmessage)
    list(GET ARGV 0 MessageType)

    if(MessageType STREQUAL FATAL_ERROR OR MessageType STREQUAL ERROR)
        list(REMOVE_AT ARGV 0)
        message(STATUS "${BoldRed}[${MessageType}]: ${ARGV}${ColorReset}")

    elseif(MessageType STREQUAL WARNING)
        list(REMOVE_AT ARGV 0)
        message(STATUS "${BoldYellow}[${MessageType}]: ${ARGV}${ColorReset}")

        # elseif(MessageType STREQUAL AUTHOR_WARNING)
        #   list(REMOVE_AT ARGV 0)
        #   message(STATUS "${BoldCyan}${ARGV}${ColorReset}")

    elseif(MessageType STREQUAL OK)
        list(REMOVE_AT ARGV 0)
        message(STATUS "${Green}[${MessageType}]: ${ARGV}${ColorReset}")

    elseif(MessageType STREQUAL INFO)
        list(REMOVE_AT ARGV 0)
        message(STATUS "${Cyan}[${MessageType}]: ${ARGV}${ColorReset}")

    else()
        message("${ARGV}")

    endif()
endfunction()


# cmessage("Plain text")
# cmessage(OK "This is good")
# cmessage(INFO "Just info")
# cmessage(WARNING "This may cause problems")
# cmessage(ERROR "This is wrong")
# cmessage(FATAL_ERROR "We are dying")
