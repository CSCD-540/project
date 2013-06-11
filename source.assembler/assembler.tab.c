/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "assembler.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char  *yytext;
FILE  *yyin;
FILE  *outfile;
int   yy_flex_debug;
int   execute;


typedef struct { 
  char labelname[4];
  int  pid;
  int  memloc;
} ILABEL;

typedef struct { 
  char label[5];
  int address;
} JSYM;

typedef struct { 
  char varname[11];
  int address;
} VTABLE;

VTABLE vtable[50];      // variable/address name table
int vtablex = 0;
int vaddress = 0;

ILABEL ilabel[10];      // store mem loc for label
int ilabelindex = 0;
int initdataindex = 0;

int initdatasection[100];
int index2initdata[20];
int index2index=0;

JSYM locjsym[30];
JSYM refjsym[30];
int refex=0;
int locex=0;

typedef struct { 
  char idop[11];
  int type;
} PARSE;

PARSE parse[100];

int yylex(void);
void yyerror(char *);

int i,j,kkkk;

int  sindex = 0;
int  pindex = 0;
int  memloc = 0;

#define MAXPRO 6          // max num of processes
#define MAXMEM 200        // max size of a process
#define STACKSIZE 100     // max size of the stack
#define REGISTERSIZE 10   // size of each process registers
#define MAXGMEM 20        // max size of global memory
#define NORMAL 0          // denotes a normal return from exe()
#define LOCKED 1          // stops process switching until unlock
#define UNLOCKED 2        // remove lock
#define ENDPROCESS 3
#define p0WRITE 4         // tells p0 to run-p0 should only run after a write to gmem

int gmem[MAXGMEM];        // global var sit here 
int mem[MAXPRO][MAXMEM]; 
int endprog[MAXPRO];      // last instruction of proc
int jsym[60]; 
int pid = 0;              // process id

int p0running;

//execute one instruction return 0 if instruction is not lock
int exe(int stack[][STACKSIZE],int sp[],int reg[][REGISTERSIZE], int next_instruct[],int next_inst[], int cur_proc);
int pop(int stack[][STACKSIZE], int proc_id, int sp[], int calledfrom);
void push(int stack[][STACKSIZE], int proc_id, int sp[],int data, int calledfrom);
void print_stack(int stack[][STACKSIZE],int sp[]); //debug
void print_register(int reg[][REGISTERSIZE]); //debug
void print_gmem();

int showparse(int beg) { 
  int i;

  for (i=beg; i<sindex; i++)   
    printf("############## i=%d  %s, (type)%d\n", i, parse[i].idop, parse[i].type);
      
  printf("end show\n");
}

#if 0
  int termcopy(int beg) { 
    int i, j;
    printf("termcopy: storage loc calc here\n");
    j = 0;        
    strcpy(parsecopy[j].idop, parse[beg].idop);
    parsecopy[j].type = parse[beg].type;
 
    printf("%s  ->   %s\n", parse[beg].idop, parsecopy[beg].idop);

    if(parse[beg+1].idop[0] == '[') { 
      i= beg+1; 
      j++;
      do { 
        /* append parse[i].idop to tgt */
        strcpy(parsecopy[j].idop, parse[i].idop);
        printf("%s  ->   %s\n", parse[i].idop, parsecopy[j].idop);
        parsecopy[j].type = parse[beg].type;
        j++;
      } while (parse[i++].idop[0] != ']');
    }

    copysindex = j;
  }
#endif

int showterm( int beg) { 
  int i;
  return;
}



/* Line 268 of yacc.c  */
#line 203 "assembler.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER = 258,
     ID = 259,
     LD = 260,
     LA = 261,
     LOAD = 262,
     LOADI = 263,
     ADD = 264,
     SUB = 265,
     MUL = 266,
     DIV = 267,
     END = 268,
     ENDP = 269,
     PROCESS = 270,
     AND = 271,
     OR = 272,
     NOT = 273,
     LE_OP = 274,
     GE_OP = 275,
     LT_OP = 276,
     GT_OP = 277,
     EQ_OP = 278,
     NE_OP = 279,
     STOP = 280,
     STOR = 281,
     ST = 282,
     DATA = 283,
     IDATA = 284,
     COMMA = 285,
     COLON = 286,
     LOCK = 287,
     UNLOCK = 288,
     OPEN = 289,
     READ = 290,
     WRITE = 291,
     CLOSE = 292,
     SEEK = 293,
     POPD = 294,
     POP = 295,
     DOT = 296,
     HALT = 297,
     JFALSE = 298,
     JMP = 299,
     LPREC = 300,
     RPREC = 301
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 132 "assembler.y"

  int intval;
  char *sptr;



/* Line 293 of yacc.c  */
#line 292 "assembler.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 304 "assembler.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   141

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  54
/* YYNRULES -- Number of states.  */
#define YYNSTATES  75

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    13,    16,    19,    22,
      24,    26,    30,    32,    35,    38,    40,    43,    47,    50,
      52,    55,    59,    62,    65,    67,    70,    73,    76,    78,
      80,    82,    84,    86,    89,    91,    94,    97,    99,   101,
     103,   105,   108,   110,   112,   114,   116,   118,   120,   122,
     124,   126,   128,   131,   133
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    49,    -1,    54,    -1,    50,    54,    -1,
      49,    54,    -1,    51,    52,    -1,    41,    28,    -1,    52,
      53,    -1,    53,    -1,    64,    -1,    64,    30,    64,    -1,
      60,    -1,    60,    55,    -1,    56,    57,    -1,    56,    -1,
      41,    29,    -1,    57,    58,    59,    -1,    58,    59,    -1,
      64,    -1,    31,    64,    -1,    61,    62,    13,    -1,    15,
      64,    -1,    62,    63,    -1,    63,    -1,     7,    64,    -1,
       8,    64,    -1,     6,    64,    -1,    34,    -1,    35,    -1,
      36,    -1,    37,    -1,    38,    -1,    39,    64,    -1,    40,
      -1,    43,    64,    -1,    44,    64,    -1,    32,    -1,    33,
      -1,     5,    -1,    27,    -1,    26,    64,    -1,    23,    -1,
      22,    -1,    20,    -1,    21,    -1,    19,    -1,     9,    -1,
      10,    -1,    16,    -1,    17,    -1,    42,    -1,    64,    31,
      -1,     4,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   188,   188,   199,   200,   201,   204,   207,   212,   213,
     216,   228,   257,   258,   261,   262,   265,   270,   271,   274,
     277,   353,   364,   376,   377,   380,   394,   402,   423,   428,
     433,   438,   443,   448,   461,   466,   475,   484,   489,   494,
     499,   504,   518,   523,   528,   533,   538,   543,   548,   553,
     558,   563,   568,   578,   583
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMBER", "ID", "LD", "LA", "LOAD",
  "LOADI", "ADD", "SUB", "MUL", "DIV", "END", "ENDP", "PROCESS", "AND",
  "OR", "NOT", "LE_OP", "GE_OP", "LT_OP", "GT_OP", "EQ_OP", "NE_OP",
  "STOP", "STOR", "ST", "DATA", "IDATA", "COMMA", "COLON", "LOCK",
  "UNLOCK", "OPEN", "READ", "WRITE", "CLOSE", "SEEK", "POPD", "POP", "DOT",
  "HALT", "JFALSE", "JMP", "LPREC", "RPREC", "$accept", "programA",
  "program", "varlistend", "dotdata", "varlists", "varlist", "program0",
  "initdata", "dotidata", "labels", "labl", "collabeldata", "program1",
  "process0", "code0", "code", "term", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    49,    49,    50,    51,    52,    52,
      53,    53,    54,    54,    55,    55,    56,    57,    57,    58,
      59,    60,    61,    62,    62,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    64,    64
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     2,     1,
       1,     3,     1,     2,     2,     1,     2,     3,     2,     1,
       2,     3,     2,     2,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     2,     1,     2,     2,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     0,     0,     3,    12,     0,
      54,    53,    22,     7,     1,     5,     4,     6,     9,    10,
       0,    13,    15,    39,     0,     0,     0,    47,    48,    49,
      50,    46,    44,    45,    43,    42,     0,    40,    37,    38,
      28,    29,    30,    31,    32,     0,    34,    51,     0,     0,
       0,    24,     0,     8,     0,    16,    14,     0,    19,    27,
      25,    26,    41,    33,    35,    36,    21,    23,    52,    11,
       0,     0,    18,    17,    20
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    17,    18,     7,    21,    22,
      56,    57,    72,     8,     9,    50,    51,    19
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -54
static const yytype_int8 yypact[] =
{
     -14,     0,   -26,     7,    -6,    -6,     0,   -54,   -31,    97,
     -54,   -54,   -54,   -54,   -54,   -54,   -54,     0,   -54,   -19,
     -17,   -54,     0,   -54,     0,     0,     0,   -54,   -54,   -54,
     -54,   -54,   -54,   -54,   -54,   -54,     0,   -54,   -54,   -54,
     -54,   -54,   -54,   -54,   -54,     0,   -54,   -54,     0,     0,
      55,   -54,   -18,   -54,     0,   -54,     0,   -16,   -54,   -54,
     -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,
     -16,     0,   -54,   -54,   -54
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -54,   -54,   -54,   -54,   -54,   -54,    -3,     1,   -54,   -54,
     -54,   -40,   -53,   -54,   -54,   -54,   -32,    -1
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      12,     1,    13,    10,    11,    15,    16,    14,    52,     1,
      20,    54,    55,    68,    53,    71,    70,    73,    67,     0,
       0,    58,     0,    59,    60,    61,     0,     2,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,    64,    65,    52,
       0,     0,     0,    69,     0,    58,     0,     0,    10,    11,
      23,    24,    25,    26,    27,    28,     0,     0,    66,     0,
      74,    29,    30,     0,    31,    32,    33,    34,    35,     0,
       0,    36,    37,     0,     0,     0,     0,    38,    39,    40,
      41,    42,    43,    44,    45,    46,     0,    47,    48,    49,
      10,    11,    23,    24,    25,    26,    27,    28,     0,     0,
       0,     0,     0,    29,    30,     0,    31,    32,    33,    34,
      35,     0,     0,    36,    37,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,    44,    45,    46,     0,    47,
      48,    49
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-54))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       1,    15,    28,     3,     4,     4,     5,     0,     9,    15,
      41,    30,    29,    31,    17,    31,    56,    70,    50,    -1,
      -1,    22,    -1,    24,    25,    26,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,
      -1,    -1,    -1,    54,    -1,    56,    -1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    13,    -1,
      71,    16,    17,    -1,    19,    20,    21,    22,    23,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    43,    44,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    16,    17,    -1,    19,    20,    21,    22,
      23,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    15,    41,    48,    49,    50,    51,    54,    60,    61,
       3,     4,    64,    28,     0,    54,    54,    52,    53,    64,
      41,    55,    56,     5,     6,     7,     8,     9,    10,    16,
      17,    19,    20,    21,    22,    23,    26,    27,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    42,    43,    44,
      62,    63,    64,    53,    30,    29,    57,    58,    64,    64,
      64,    64,    64,    64,    64,    64,    13,    63,    31,    64,
      58,    31,    59,    59,    64
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 188 "assembler.y"
    { 
      int j,k;
      printf("END of ProgramA\n");
      for(j=0; j<ilabelindex; j++) {
        printf("pid = %d\n", ilabel[j].pid );
         printf("memloc = %d\n", ilabel[j].memloc);
         printf("name = %s\n", ilabel[j].labelname);
      }
  }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 200 "assembler.y"
    {; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 201 "assembler.y"
    {;}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 204 "assembler.y"
    {;}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 207 "assembler.y"
    { 
      sindex=0;
    }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 216 "assembler.y"
    { 
      int i = 0;
      strcpy(vtable[vtablex].varname, parse[i].idop);
      vtable[vtablex].address=vaddress;
      vtablex++;
      vaddress++;

      showsymtable();
                
      gmem[memloc++]=0;
      sindex = 0;
    }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 228 "assembler.y"
    { 
      int i,j;

      printf("term comma term\n");
      showparse(0);
      printf("vtablex=%d vaddress:%d\n", vtablex, vaddress);

      strcpy(vtable[vtablex].varname, parse[0].idop);

      vtable[vtablex].address=vaddress;
      sscanf(parse[2].idop, "%d", &i);

      printf("vtable[%d].varname=%s, parse[0].idop=%s\n", 
      vtablex, vtable[vtablex].varname,    parse[0].idop);
      printf(" vtable[vtablex].address=%d\n", vtablex, vtable[vtablex].address);
   
      vaddress += i;
      vtablex++;

      showsymtable();
      printf("vtablex=%d vaddress:%d\n", vtablex, vaddress);

      for(j = 0; j < i; j++)  
        gmem[memloc++]=0;
      
      sindex = 0;
    }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 261 "assembler.y"
    {}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 262 "assembler.y"
    {}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 265 "assembler.y"
    { 
      sindex=0;
    }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 271 "assembler.y"
    {}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 274 "assembler.y"
    {}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 277 "assembler.y"
    { 
      int i, j, k;
      int ch; 
      char *chptr;

      printf("collabeldata\n");
      showparse(0);
      printf("==================================\n");
      printf("parse[0].idop=%s\n", parse[0].idop);
      printf("parse[2].idop=%s\n", parse[2].idop);

      for(j=0; j<ilabelindex; j++) {
        printf("pid = %d\n", ilabel[j].pid );
        printf("memloc = %d\n", ilabel[j].memloc);
        printf("name = %s\n", ilabel[j].labelname);

        if( ilabel[j].memloc != -999) { 
          printf("compare ilabel's %s  and label parsed %s\n", ilabel[j].labelname,  parse[0].idop); 
        
          if( strcmp(ilabel[j].labelname ,  parse[0].idop)==0) { 
            printf("label found, endprog is at  %d\n", endprog[ ilabel[j].pid] );
            chptr =  parse[2].idop;
            for( ; *chptr; chptr++) 
              printf("%c", *chptr);

            printf("\n");

            chptr =  parse[2].idop;
            mem[ ilabel[j].pid][ ilabel[j].memloc ] = endprog[ilabel[j].pid];
                 
            for( ; *chptr; chptr++) { 
              printf("at %d: locating '%c'\n", endprog[ilabel[j].pid], *chptr);
              mem[ilabel[j].pid][endprog[ilabel[j].pid]] = *chptr;
              endprog[ilabel[j].pid]++;
            }

           mem[ ilabel[j].pid][endprog[ilabel[j].pid]] = 0;
           endprog[ilabel[j].pid]++;
           ilabel[j].memloc = -999;
         } 
       }

    }

    printf("==================================\n");
    printf("==================================\n");
    
    for(j=0; j<ilabelindex; j++)
      printf("pid = %d  end lo = %d\n", ilabel[j].pid, endprog[ilabel[j].pid]);
    
    printf("==================================\n");

    // allocate parse[2].idop to initdatasection[]
    // and backpatch Li labels. 

    printf("index2initdata[%d] =  %d\n", index2index, initdataindex);       
    index2initdata[index2index]=initdataindex;       

    for (k=0; ch = *((char *)(parse[2].idop)+k); k++)
      initdatasection[initdataindex++]= ch;
    
    index2index++;
 
    for(k=0; k<initdataindex; k++)
      printf("%c", initdatasection[k]);
    
    printf("\n");
    
    for(k = 0; k < index2index; k++)
      printf("%d  ", index2initdata[k]);
    
    printf("\n==================================\n");
    sindex = 0;
  }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 353 "assembler.y"
    {
       mem[pid][memloc++]=END;
       sindex=0;
       endprog[pid]=memloc; // last instruction location+1
       printf("at END refex=%d locex=%d\n", refex, locex);
       showjsym(); 
       refex = locex = 0;
       pid++;
     }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 364 "assembler.y"
    {
      showterm(0);
      sindex = 0; 
      memloc = 10;
      
      if(pid > MAXPRO) {
        fprintf(stderr,"max num of proccesses currently 6\n");
        exit(1);
      }
    }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 380 "assembler.y"
    {
      int i, j;
      mem[pid][memloc++] = LOAD;
      i = searchvtable(parse[0].idop, &j, 1);
      if ( i >= 230 ) 
         mem[pid][memloc++]=i;
      else {
         printf("vtable[%d].address=%d\n",i, vtable[i].address);
         mem[pid][memloc++]=vtable[i].address;
       }

       showterm(0);
       sindex = 0;
    }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 394 "assembler.y"
    {
      int i;
      mem[pid][memloc++] = LOADI;
      sscanf(parse[0].idop, "%d", &i);
      mem[pid][memloc++]=i;
      showterm(0);
      sindex = 0;
    }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 402 "assembler.y"
    { 
      int i, j;
      mem[pid][memloc++] = LA;
      printf("LA term parse[0].idop = %s\n",parse[0].idop); 
      i = searchvtable(parse[0].idop, &j, 2);
      printf("LA term searchvtable ret val= %d\n", i); 

      if( i == -999) { 
        ilabel[ilabelindex].pid = pid;
        ilabel[ilabelindex].memloc = memloc;
        sprintf(ilabel[ilabelindex].labelname, "%s",  parse[0].idop);
        ilabelindex++;
        mem[pid][memloc++] = -999;
        printf("wrote mem[%d][%d] =  %d\n", pid, memloc-1, mem[pid][memloc-1]); 
      } else {
        mem[pid][memloc++]=i;
      }
      
      showterm(0);
      sindex = 0;
    }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 423 "assembler.y"
    {
      mem[pid][memloc++]=OPEN;
      showterm(0);
      sindex = 0;
    }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 428 "assembler.y"
    {
      mem[pid][memloc++]=READ;
      showterm(0);
      sindex = 0;
    }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 433 "assembler.y"
    {
      mem[pid][memloc++]=WRITE;
      showterm(0);
      sindex = 0;
    }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 438 "assembler.y"
    {
      mem[pid][memloc++]=CLOSE;
      showterm(0);
      sindex = 0;
    }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 443 "assembler.y"
    {
      mem[pid][memloc++]=SEEK;
      showterm(0);
      sindex = 0;
    }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 448 "assembler.y"
    {
      int i, j;
      showterm(0);
      mem[pid][memloc++] = POPD;
      i = searchvtable(parse[0].idop, &j, 31);
      if( i >= 230 ) 
        mem[pid][memloc++]=i;
      else 
        mem[pid][memloc++]=vtable[i].address;
          
      showterm(0);
      sindex = 0;
    }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 461 "assembler.y"
    {
      mem[pid][memloc++]=POP;
      showterm(0);
      sindex = 0;
    }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 466 "assembler.y"
    { 
      mem[pid][memloc++]=JFALSE;
      mem[pid][memloc]=-1;
      refjsym[refex].address = memloc++;
      sprintf( refjsym[refex].label, "%s", parse[0].idop);
      refex++;
      showterm(0);
      sindex = 0;
    }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 475 "assembler.y"
    {
      mem[pid][memloc++] = JMP;
      mem[pid][memloc] = -1;
      refjsym[refex].address = memloc++;
      sprintf( refjsym[refex].label, "%s", parse[0].idop);
      refex++;
      showterm(0);
      sindex = 0;
    }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 484 "assembler.y"
    {
      mem[pid][memloc++]=LOCK;
      showterm(0);
      sindex = 0;
    }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 489 "assembler.y"
    {
      mem[pid][memloc++]=UNLOCK;
      showterm(0);
      sindex = 0;
    }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 494 "assembler.y"
    {
      mem[pid][memloc++]=LD;
      showterm(0);
      sindex = 0;
    }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 499 "assembler.y"
    {
      mem[pid][memloc++]=ST;
      showterm(0);
      sindex = 0;
    }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 504 "assembler.y"
    {
      int i, j;
      showterm(0);
      mem[pid][memloc++]=STOR;
      i = searchvtable(parse[0].idop, &j, 3);
   
      if( i >= 230 )  
        mem[pid][memloc++] = i;
      else
        mem[pid][memloc++] = i;

      showterm(0);
      sindex = 0;
    }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 518 "assembler.y"
    {
      mem[pid][memloc++] = EQ_OP;
      showterm(0);
      sindex = 0;
    }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 523 "assembler.y"
    { 
      mem[pid][memloc++]=GT_OP;
      showterm(0);
      sindex = 0;
    }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 528 "assembler.y"
    {
      mem[pid][memloc++]=GE_OP;
      showterm(0);
      sindex = 0;
    }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 533 "assembler.y"
    {
      mem[pid][memloc++]=LT_OP;
      showterm(0);
      sindex = 0;
    }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 538 "assembler.y"
    {
      mem[pid][memloc++]=LE_OP;
      showterm(0);
      sindex = 0;
    }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 543 "assembler.y"
    {
      mem[pid][memloc++]=ADD;
      showterm(0);
      sindex = 0;
    }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 548 "assembler.y"
    {
      mem[pid][memloc++]=SUB;
      showterm(0);
      sindex = 0;
    }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 553 "assembler.y"
    {
      mem[pid][memloc++]=AND;
      showterm(0);
      sindex = 0;
    }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 558 "assembler.y"
    {
      mem[pid][memloc++]=OR;
      showterm(0);
      sindex = 0;
    }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 563 "assembler.y"
    {
      mem[pid][memloc++]=HALT;
      showterm(0);
      sindex = 0;
    }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 568 "assembler.y"
    {
      locjsym[locex].address = memloc;
      sprintf( locjsym[locex].label, "%s", parse[0].idop);
      locex++;
      showterm(0);
      sindex = 0;
    }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 578 "assembler.y"
    {
      sprintf( parse[sindex].idop, "%s", yylval.sptr);
      parse[sindex].type = ID;
      sindex++;
    }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 583 "assembler.y"
    {
      sprintf( parse[sindex].idop, "%s", yylval.sptr);
      parse[sindex].type = NUMBER;
      sindex++;
    }
    break;



/* Line 1806 of yacc.c  */
#line 2242 "assembler.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 589 "assembler.y"


main(int argc, char **argv) {
  printf("***********************\n");
  printf("      assembler\n");
  printf("***********************\n");
  
  if(argc != 3) { 
    fprintf(stderr, "usage: assembler <input> <output> \n");
    fprintf(stderr, "output: <output>.cpu \n");
    exit(0);
  }

  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("cwd: %s \n", cwd);

  printf("loading: %s \n", argv[1]);
  // global file pointer
  yyin = fopen(argv[1], "r" );

  printf("attempting to parse...\n");
  // bison function
  yyparse();
  
  printf("attempting to export...\n");
  printf("saving: %s\n", argv[2]);
  exportMemory(argv[2]);

  fclose(yyin);
  
  return 0;
}

void yyerror(char *s) {
  extern unsigned int linenumber;
  fprintf(stderr, "%s\n", s);
  fprintf(stderr, "line %d:  %s\n", linenumber+1, yytext);
  return;
}

showsymtable() {
  int i;
  for (i=0; i<vtablex; i++) {  
    printf("%d:  %s\n",
    vtable[i].address,  vtable[i].varname);
  }
}

int searchvtable(char *str, int *j, int from) {
  int i, found;

  printf("--------search vtable for %s  from=%d\n", str, from);
  printf("%c %c\n", *str, *(str+1));

  if( *str =='r' && ( '0' <= *(str+1) && *(str+1) <= '9' )) { 
    printf("this is local var\n");
    
    // 230 or above is locar var, r?
    return( (*(str+1) - '0')+230 ); 
  }

  for(i=found=0; i<vtablex; i++) {
    if(strcmp(vtable[i].varname, str)==0) { 
      found = 1;
      *j = i; 
      break;
    }
  }
  
  if(found) {
    ;
  } else {
    return (-999);
  }

  printf("---------search vtable for %s ended\n", str);
  printf("----------returning %d   and  j= %d\n", vtable[i].address, *j);
  return( vtable[i].address);
}

showjsym() { 
  int i, j;
  
  printf("SHOWSYM %d\n", execute);
  
  for (i = 0; i < locex; i++) {
    if (execute == 0) 
      printf("%s  %d\n", locjsym[i].label, locjsym[i].address );
  }

  printf("SHOWSYM refex %d\n", refex);
  
  for (i = 0; i < refex; i++) {
    if (execute == 0) 
      printf("%s  %d", refjsym[i].label, refjsym[i].address );
   
    if (execute == 0) 
      printf(" --  %d\n", mem[pid][ refjsym[i].address] );
  }
  
  /* search refjsym in locjsym, fill with locjsym.address */  
  for (i = 0; i < refex; i++) {
    printf("%s  %d", refjsym[i].label, refjsym[i].address );
    printf(" --  %d\n", mem[pid][ refjsym[i].address] );
    
    /* search refjsym in locjsym, fill with locjsym.address */  
    if(mem[pid][ refjsym[i].address] != -1)
      if(execute==0)   
        printf("jump lable wrong %s\n", refjsym[i].label);
    
    for(j = 0; j < locex; j++) {

      if(strcmp(locjsym[j].label, refjsym[i].label) == 0) {
        mem[pid][ refjsym[i].address] = locjsym[j].address; 
        break;
      }
    }
    
    if(j >= locex)
     if(execute==0) 
       fprintf(stderr,"lable not found: %s\n", refjsym[i].label);
  }
  
  printf("DONE showsym\n");  
}


int searchsym( char *str, int j) {
  int i;

  for(i = 0; i < vtablex; i++) {
    if(vtable[i].address == j ) { 
      sscanf(vtable[i].varname, "%s", str);
      return; 
    }
  }
  
  fprintf(stderr, " symbol %s not found, error\n", str);
  fprintf(stderr, " this may be initdata label.\n");

  printf("---------search vtable for %s ended\n", str);
}


exportMemory(char* filename) {
  int i;
  int j;
  
  strcat(filename, ".cpu");
  
  FILE *fd;
  fd = fopen(filename, "w");
  
  
  printf("************************************\n");
  printf("  Exporting Memory: %s\n", filename);
  printf("************************************\n");
      
  fprintf(fd, "processes: %d \n", pid);

  fprintf(fd, "size: ");
  for (i = 0; i < pid; i++)
    fprintf(fd, "%d ", endprog[i]);
  fprintf(fd, "\n");

  for (i = 0; i < pid; i++) {
    for (j = 0; j <= endprog[i]; j++)
      fprintf(fd, "%d ", mem[i][j]);
    fprintf(fd, "\n");
  }
  
}


// change printf to fprintf to print to file
showmem() { 
  int i;
  int k; 
  char name[11];
  int progid;
  
  for (k = 0; k < pid; k++) {
    printf("############### pid %d ############\n", k);
    printf("############### endprog %d ############\n", endprog[k]);
  
    // endprog[] contains last instruction addrs of proc
    for (i = 0; i < endprog[k]; i++) { 
      switch (mem[k][i] ) {

        case POPD:
          if (mem[k][i+1]<230) { 
            searchsym(name, mem[k][i + 1]);
            printf("%04d:  POPD\t %d  (%s)\n", i, mem[k][i+1], name);
          } else
            printf("%04d:  POPD\t %d\n", i, mem[k][i+1]); 
          
          i++;
          break;

        case POP :
          printf("%04d:  POP\n", i); 
          break;

        case OPEN :
          printf("%04d:  OPEN\n", i); 
          break;

        case READ :
          printf("%04d:  READ\n", i); 
          break;

        case WRITE :
          printf("%04d:  WRITE\n", i); 
          break;

        case SEEK :
          printf("%04d:  SEEK\n", i); 
          break;
          
        case CLOSE :
          printf("%04d:  CLOSE\n", i); 
          break;

        case LD :
          printf("%04d:  LD\n", i); 
          break;

        case LA :
          searchsym(name, mem[k][i+1]);
          printf("%04d:  LA\t %d (%s)\n", i, mem[k][i+1], name); i++;
          break;

        case LOAD :
          if(mem[k][i+1]<230) { 
            searchsym(name, mem[k][i+1]);
            printf("%04d:  LOAD\t %d  (%s)\n", i, mem[k][i+1], name);
          } else
            printf("%04d:  LOAD\t %d\n", i, mem[k][i+1]); 
          i++;
          break;

        case LOADI :
          printf("%04d:  LOADI\t %d\n", i, mem[k][i+1]); i++;
          break;

        case ADD:
          printf("%04d:  ADD\n", i); 
          break;

        case SUB :
          printf("%04d:  SUB\n", i); 
          break;

        case MUL :
          printf("%04d:  MUL\n", i); 
          break;

        case DIV :
          printf("%04d:  DIV\n", i); 
          break;
          case END:
          printf("%04d:  END\n", i); 
          break;

        case ENDP:
          printf("%04d:  ENDP\n", i); 
          break;

        case AND:
          printf("%04d:  AND\n", i); 
          break;

        case OR:
          printf("%04d:  OR\n", i); 
          break;

        case NOT:
          printf("%04d:  NOT\n", i); 
          break;

        case LE_OP:
          printf("%04d:  LE_OP\n", i); 
          break;
          case GE_OP:
          printf("%04d:  GE_OP\n", i); 
          break;

        case LT_OP:
          printf("%04d:  LT_OP\n", i); 
          break;
          case GT_OP:
          printf("%04d:  GT_OP\n", i); 
          break;

        case EQ_OP:
          printf("%04d:  EQ_OP\n", i); 
          break;

        case NE_OP:
          printf("%04d:  NE_OP\n", i); 
          break;

        case STOP :
          printf("%04d:  STOP\n", i); 
          break;

        case STOR:
          if(mem[k][i+1]<230) { 
            searchsym(name, mem[k][i+1]);
            printf("%04d:  STOR\t %d  (%s)\n", i, mem[k][i+1], name);
          } else
            printf("%04d:  STOR\t %d\n", i, mem[k][i+1]); 
          
          i++;
          break;

        case ST :
          printf("%04d:  ST\n", i); 
          break;

        case LOCK :
          printf("%04d:  LOCK\n", i); 
          break;

        case UNLOCK :
          printf("%04d:  UNLOCK\n", i); 
          break;

        case HALT :
          printf("%04d:  HALT\n", i); 
          break;

        case JFALSE :
          printf("%04d:  JFALSE\t %d\n",  i, mem[k][i+1]); i++; 
          break;

        case JMP: 
          printf("%04d:  JMP\t %d\n", i, mem[k][i+1]); i++; 
          break;

        default:
          printf("(%04d:   %d)\n", i, mem[k][i]);  
          break;
      }
    }
  } // end of printing each proc memory

}
