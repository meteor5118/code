/* ====================================================================
 * Copyright (c) 2015.  All rights reserved.
 *
 * Author: xxx
 *
 * ====================================================================
 */

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include <unistd.h>
#include <getopt.h>

using namespace std;

struct LongShort {
  const char *letter; /* short name option */
  const char *lname;  /* long name option */
  int extraparam;    /* whether it takes an additional argument */
};

#define TRUE 1
#define FALSE 0

static const struct LongShort aliases[]= {
  /* all these ones, starting with "*" or "$" as a short-option have *no*
     short option to mention. */
  {"*",  "url",                      TRUE},
  {"*4", "dns-ipv4-addr",            TRUE},
  {"*6", "dns-ipv6-addr",            TRUE},
  {"*a", "random-file",              TRUE},
  {"*b", "egd-file",                 TRUE},
  {"*B", "oauth2-bearer",             TRUE},
  {"*c", "connect-timeout",          TRUE},
  {"*d", "ciphers",                  TRUE},
  {"*D", "dns-interface",            TRUE},
  {"*e", "disable-epsv",             FALSE},
  {"*E", "epsv",                     FALSE},
         /* 'epsv' made like this to make --no-epsv and --epsv to work
             although --disable-epsv is the documented option */
  {"*f", "environment",              FALSE},
  {"*F", "dns-servers",              TRUE},
  {"*g", "trace",                    TRUE},
  {"*G", "npn",                      FALSE},
  {"*h", "trace-ascii",              TRUE},
  {"*H", "alpn",                     FALSE},
  {"*i", "limit-rate",               TRUE},
  {"*j", "compressed",               FALSE},
  {"*J", "tr-encoding",              FALSE},
  {"*k", "digest",                   FALSE},
  {"*l", "negotiate",                FALSE},
  {"*m", "ntlm",                     FALSE},
  {"*M", "ntlm-wb",                  FALSE},
  {"*n", "basic",                    FALSE},
  {"*o", "anyauth",                  FALSE},
  {"*p", "wdebug",                   FALSE},
  {"*q", "ftp-create-dirs",          FALSE},
  {"*r", "create-dirs",              FALSE},
  {"*s", "max-redirs",               TRUE},
  {"*t", "proxy-ntlm",               FALSE},
  {"*u", "crlf",                     FALSE},
  {"*v", "stderr",                   TRUE},
  {"*w", "interface",                TRUE},
  {"*x", "krb" ,                     TRUE},
  {"*x", "krb4" ,                    TRUE},
         /* 'krb4' is the previous name */
  {"*y", "max-filesize",             TRUE},
  {"*z", "disable-eprt",             FALSE},
  {"*Z", "eprt",                     FALSE},
         /* 'eprt' made like this to make --no-eprt and --eprt to work
             although --disable-eprt is the documented option */
  {"$a", "ftp-ssl",                  FALSE},
         /* 'ftp-ssl' deprecated name since 7.20.0 */
  {"$a", "ssl",                      FALSE},
         /* 'ssl' new option name in 7.20.0, previously this was ftp-ssl */
  {"$b", "ftp-pasv",                 FALSE},
  {"$c", "socks5",                   TRUE},
  {"$c", "socks",                    TRUE},
         /* 'socks' is how the option once was documented but we prefer
            the --socks5 version for explicit version */
  {"$d", "tcp-nodelay",              FALSE},
  {"$e", "proxy-digest",             FALSE},
  {"$f", "proxy-basic",              FALSE},
  {"$g", "retry",                    TRUE},
  {"$h", "retry-delay",              TRUE},
  {"$i", "retry-max-time",           TRUE},
  {"$k", "proxy-negotiate",          FALSE},
  {"$m", "ftp-account",              TRUE},
  {"$n", "proxy-anyauth",            FALSE},
  {"$o", "trace-time",               FALSE},
  {"$p", "ignore-content-length",    FALSE},
  {"$q", "ftp-skip-pasv-ip",         FALSE},
  {"$r", "ftp-method",               TRUE},
  {"$s", "local-port",               TRUE},
  {"$t", "socks4",                   TRUE},
  {"$T", "socks4a",                  TRUE},
  {"$u", "ftp-alternative-to-user",  TRUE},
  {"$v", "ftp-ssl-reqd",             FALSE},
         /* 'ftp-ssl-reqd' deprecated name since 7.20.0 */
  {"$v", "ssl-reqd",                 FALSE},
         /* 'ssl-reqd' new in 7.20.0, previously this was ftp-ssl-reqd */
  {"$w", "sessionid",                FALSE},
         /* 'sessionid' listed as --no-sessionid in the help */
  {"$x", "ftp-ssl-control",          FALSE},
  {"$y", "ftp-ssl-ccc",              FALSE},
  {"$j", "ftp-ssl-ccc-mode",         TRUE},
  {"$z", "libcurl",                  TRUE},
  {"$#", "raw",                      FALSE},
  {"$0", "post301",                  FALSE},
  {"$1", "keepalive",                FALSE},
         /* 'keepalive' listed as --no-keepalive in the help */
  {"$2", "socks5-hostname",          TRUE},
  {"$3", "keepalive-time",           TRUE},
  {"$4", "post302",                  FALSE},
  {"$5", "noproxy",                  TRUE},
  {"$6", "socks5-gssapi-service",    TRUE},
  {"$7", "socks5-gssapi-nec",        FALSE},
  {"$8", "proxy1.0",                 TRUE},
  {"$9", "tftp-blksize",             TRUE},
  {"$A", "mail-from",                TRUE},
  {"$B", "mail-rcpt",                TRUE},
  {"$C", "ftp-pret",                 FALSE},
  {"$D", "proto",                    TRUE},
  {"$E", "proto-redir",              TRUE},
  {"$F", "resolve",                  TRUE},
  {"$G", "delegation",               TRUE},
  {"$H", "mail-auth",                TRUE},
  {"$I", "post303",                  FALSE},
  {"$J", "metalink",                 FALSE},
  {"$K", "sasl-ir",                  FALSE},
  {"$L", "test-event",               FALSE},
  {"$M", "unix-socket",              TRUE},
  {"$N", "path-as-is",               FALSE},
  {"$O", "proxy-service-name",       TRUE},
  {"$P", "service-name",             TRUE},
  {"$Q", "proto-default",            TRUE},
  {"0",   "http1.0",                 FALSE},
  {"01",  "http1.1",                 FALSE},
  {"02",  "http2",                   FALSE},
  {"1",  "tlsv1",                    FALSE},
  {"10",  "tlsv1.0",                 FALSE},
  {"11",  "tlsv1.1",                 FALSE},
  {"12",  "tlsv1.2",                 FALSE},
  {"2",  "sslv2",                    FALSE},
  {"3",  "sslv3",                    FALSE},
  {"4",  "ipv4",                     FALSE},
  {"6",  "ipv6",                     FALSE},
  {"a",  "append",                   FALSE},
  {"A",  "user-agent",               TRUE},
  {"b",  "cookie",                   TRUE},
  {"B",  "use-ascii",                FALSE},
  {"c",  "cookie-jar",               TRUE},
  {"C",  "continue-at",              TRUE},
  {"d",  "data",                     TRUE},
  {"dr", "data-raw",                 TRUE},
  {"da", "data-ascii",               TRUE},
  {"db", "data-binary",              TRUE},
  {"de", "data-urlencode",           TRUE},
  {"D",  "dump-header",              TRUE},
  {"e",  "referer",                  TRUE},
  {"E",  "cert",                     TRUE},
  {"Ea", "cacert",                   TRUE},
  {"Eb", "cert-type",                TRUE},
  {"Ec", "key",                      TRUE},
  {"Ed", "key-type",                 TRUE},
  {"Ee", "pass",                     TRUE},
  {"Ef", "engine",                   TRUE},
  {"Eg", "capath ",                  TRUE},
  {"Eh", "pubkey",                   TRUE},
  {"Ei", "hostpubmd5",               TRUE},
  {"Ej", "crlfile",                  TRUE},
  {"Ek", "tlsuser",                  TRUE},
  {"El", "tlspassword",              TRUE},
  {"Em", "tlsauthtype",              TRUE},
  {"En", "ssl-allow-beast",          FALSE},
  {"Eo", "login-options",            TRUE},
  {"Ep", "pinnedpubkey",             TRUE},
  {"Eq", "cert-status",              FALSE},
  {"Er", "false-start",              FALSE},
  {"Es", "ssl-no-revoke",            FALSE},
  {"f",  "fail",                     FALSE},
  {"F",  "form",                     TRUE},
  {"Fs", "form-string",              TRUE},
  {"g",  "globoff",                  FALSE},
  {"G",  "get",                      FALSE},
  {"h",  "help",                     FALSE},
  {"H",  "header",                   TRUE},
  {"Hp", "proxy-header",             TRUE},
  {"i",  "include",                  FALSE},
  {"I",  "head",                     FALSE},
  {"j",  "junk-session-cookies",     FALSE},
  {"J",  "remote-header-name",       FALSE},
  {"k",  "insecure",                 FALSE},
  {"K",  "config",                   TRUE},
  {"l",  "list-only",                FALSE},
  {"L",  "location",                 FALSE},
  {"Lt", "location-trusted",         FALSE},
  {"m",  "max-time",                 TRUE},
  {"M",  "manual",                   FALSE},
  {"n",  "netrc",                    FALSE},
  {"no", "netrc-optional",           FALSE},
  {"ne", "netrc-file",               TRUE},
  {"N",  "buffer",                   FALSE},
         /* 'buffer' listed as --no-buffer in the help */
  {"o",  "output",                   TRUE},
  {"O",  "remote-name",              FALSE},
  {"Oa", "remote-name-all",          FALSE},
  {"p",  "proxytunnel",              FALSE},
  {"P",  "ftpport",                  TRUE},
         /* 'ftpport' old version */
  {"P",  "ftp-port",                 TRUE},
  {"q",  "disable",                  FALSE},
  {"Q",  "quote",                    TRUE},
  {"r",  "range",                    TRUE},
  {"R",  "remote-time",              FALSE},
  {"s",  "silent",                   FALSE},
  {"S",  "show-error",               FALSE},
  {"t",  "telnet-options",           TRUE},
         /* 'telnet-options' documented as telnet-option */
  {"T",  "upload-file",              TRUE},
  {"u",  "user",                     TRUE},
  {"U",  "proxy-user",               TRUE},
  {"v",  "verbose",                  FALSE},
  {"V",  "version",                  FALSE},
  {"w",  "write-out",                TRUE},
  {"x",  "proxy",                    TRUE},
  {"X",  "request",                  TRUE},
  {"X",  "http-request",             TRUE},
         /* 'http-request' OBSOLETE VERSION */
  {"Y",  "speed-limit",              TRUE},
  {"y",  "speed-time",               TRUE},
  {"z",  "time-cond",                TRUE},
  {"#",  "progress-bar",             FALSE},
  {":",  "next",                     FALSE},
  {"~",  "xattr",                    FALSE},
  {NULL, NULL, FALSE}
};

int parse_curl() {
  const struct LongShort *p = &aliases[0];
  std::string sh_opt;
  char c;

  while (p->letter) {
    if (p->letter[0] == '*' || p->letter[0] == '$') {

    } else {
      c = p->letter[0];
    }

    if (sh_opt.find(c) == string::npos) {
      sh_opt.push_back(c);

      if (p->extraparam)
        sh_opt.push_back(':');
    }

    //printf("{\"%s\", %d, NULL, 0},\n", p->lname, p->extraparam);
    p++;
  }

  cout << sh_opt << endl;

  return 0;
}
