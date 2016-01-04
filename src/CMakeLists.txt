add_subdirectory(socket)
add_executable(pingerserver config.h pingerserver.c ui.c log.c utils.c cfg.c)
target_link_libraries (pingerserver LINK_PUBLIC -pthread -lm socket)
