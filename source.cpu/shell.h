#ifndef SHELL_H
#define SHELL_H

void shell_ls(char line[], char* sm_space);

void shell_open(char line[], char* sm_space);

void shell_rm(char line[], char* sm_space);

void shell_copy(char line[], char* sm_space);

void shell_import(char line[], char* sm_space);

void shell_load(char line[], char* sm_space);

void shell_run(char line[], char* sm_space);

void shell_dump(char line[], char* sm_space);

void shell_cat(char line[], char* sm_space);

void shell_help(char line[], char* sm_space);

#endif