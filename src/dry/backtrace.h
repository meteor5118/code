/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   backtrace.h
 * Author: Administrator
 *
 * Created on 2016年3月15日, 下午4:17
 */

#ifndef BACKTRACE_H
#define BACKTRACE_H

/*
 * get process' stack trace using libunwind-ptrace
 */
int backtrace_ptrace(int pid, int *tids, int *index, int nr_tids);

#endif /* BACKTRACE_H */

