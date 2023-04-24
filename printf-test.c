#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

int	ft_fprintf(int fd, const char *fmt, ...);
int	ft_printf(const char *fmt, ...);
int fd[2];

#define READ		0
#define WRITE		1
#define ERROR		2

#define INVOKE_TEST(test) \
	cout("[ RUN      ] %s\n", #test); \
	test(); \
	cout("[  PASSED  ]\n");

#define ASSERT_STRING_EUAL(fmt, a, b, d1, d2) \
	if (DEBUG)\
	    cout("input:'%s'\n	expected ===> '%s'\n	actual   ===> '%s'\n",fmt, a, b);	\
	if ((d1 != d2) || strlen(a) != strlen(b) || memcmp(a,b,strlen(a)  + 1) != 0)\
	{\
		cout("[  ERROR   ]:%d '%s'\n    expected: %d '%s'\n      actual: %d '%s'\n",__LINE__,fmt,d1,a, d2,b); \
		exit(1);\
	}

#define INVOKE_FLUSH(buff, r, d) \
	fflush(stdout);		\
	write(1, "", 1);	\
	if ((r = read(fd[READ], buff, sizeof(buff) - 1)) < 0) \
		perror("read"), exit(-1); \
	buff[r] = 0;

#define ASSERT_PRINTF(fmt,buf_a, buf_b, d1, d2, r1, r2,...) \
	d1 = printf(fmt,##__VA_ARGS__);\
	INVOKE_FLUSH(buf_a, r1, d1);\
	d2 = ft_printf(fmt, ##__VA_ARGS__); \
	INVOKE_FLUSH(buf_b, r2, d2)\
	ASSERT_STRING_EUAL(fmt, buf_a, buf_b,d1,d2);

#define SWAP_FD(fd)\
	if (pipe(fd) != 0)\
		perror("pipe"), exit(-1);\
	dup2(fd[WRITE], STDOUT_FILENO);\
	close(fd[WRITE]);\
	setbuf(stdout, NULL);

int output_fd = -1;
static void	cout(const char *f, ...)
{
	va_list		ap;

	if (output_fd == -1)
		output_fd = open("/dev/tty", O_RDWR);

	va_start(ap, f);
	vdprintf(output_fd, f, ap);
	va_end(ap);
}

static void test_d()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2; //d1 

	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MIN);
	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MIN);

	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("%012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-+.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-+100.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);

	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("% 012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% +012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -+.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -+100.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);

	ASSERT_PRINTF("%+ d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+ d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("%+ 012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% +012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -+100.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
//////////////////////////////////////////////////////////////////////////////////////////
	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MIN);
	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MIN);

	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("%012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-+.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-+100.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);

	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("% 012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% +012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -+.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -+100.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);

	ASSERT_PRINTF("%+ d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+ d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("%+ 012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% +012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -012d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -+100.12d",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);

	ASSERT_PRINTF("%12.10d",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%12.14d",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%14.10d",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);
	
	ASSERT_PRINTF("%-20.3d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%-31.20d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	
	ASSERT_PRINTF("%+-10.0d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+-10.2d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+-10.11d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);

	ASSERT_PRINTF("%+- 10.0d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+- 10.2d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+- 10.11d",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);

//////////////////////////////

	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("% d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%#d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 123);
	ASSERT_PRINTF("%d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%#d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

	ASSERT_PRINTF("%+d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.0d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.1d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.2d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.3d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	
    ASSERT_PRINTF("%.0d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.1d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.1dX",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.11d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
                       
    ASSERT_PRINTF("%-d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-0d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-1d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-10d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-11d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-12d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
                       
    ASSERT_PRINTF("%-d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 1236717);
    ASSERT_PRINTF("%-0d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 21323);
    ASSERT_PRINTF("%-1d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, -1);
    ASSERT_PRINTF("%-10d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, -1234567801);
                       
                       
    ASSERT_PRINTF("%-.0d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
    ASSERT_PRINTF("%-.1d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
    ASSERT_PRINTF("%-.2d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
    ASSERT_PRINTF("%-.3d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
                       
                       
    ASSERT_PRINTF("%-1.0d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
    ASSERT_PRINTF("%-2.1d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
    ASSERT_PRINTF("%-3.2d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
    ASSERT_PRINTF("%-4.3d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
                       
    ASSERT_PRINTF("%-1.0d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, -1233);
    ASSERT_PRINTF("%-2.1d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, -14121412);
    ASSERT_PRINTF("%-3.2d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 12315415)
    ASSERT_PRINTF("%-4.3d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, -1);
    ASSERT_PRINTF("%-10.3d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 1);
	ASSERT_PRINTF("%-11.19d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, -9999999);
	ASSERT_PRINTF("%-19.11d",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 999999);
    ASSERT_PRINTF("%032.31d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,  -1674665472);
}

static void test_u()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2; //d1 

	ASSERT_PRINTF("%u",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%u",printf_buff, ftprintf_buff, d1, d2 , r1, r2,UINT_MAX);
	ASSERT_PRINTF("%0u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%01u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%02u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%03u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%0u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%01u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%02u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%03u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

//////////////////////////////////////
	ASSERT_PRINTF("%.0u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.1u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.2u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.3u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.4u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);

	ASSERT_PRINTF("%.u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.1u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.2u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.3u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.4u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

//////////////////////////////////////////
	ASSERT_PRINTF("%0.0u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%0.0u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%3.1u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%3.1u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);


	ASSERT_PRINTF("%1.3u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%1.3u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	
	ASSERT_PRINTF("%20.3u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%20.3u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);


	ASSERT_PRINTF("%10.5u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 10);
	ASSERT_PRINTF("%30.5u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

	ASSERT_PRINTF("%-30.5u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	
	ASSERT_PRINTF("%-10.0u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 10);
	ASSERT_PRINTF("%-10.1u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 19);


	ASSERT_PRINTF("%-30.1u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%+u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-+100u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-+0100u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%#-+2u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 123);
	ASSERT_PRINTF("%#-+2u",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 123);
	
}



static void test_percent()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2;
	ASSERT_PRINTF("%%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
	ASSERT_PRINTF("%3%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
	ASSERT_PRINTF("%03%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
	ASSERT_PRINTF("%-3%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
	ASSERT_PRINTF("%.3%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
	ASSERT_PRINTF("%.0%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
	ASSERT_PRINTF("%0%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
	ASSERT_PRINTF("%-%",printf_buff, ftprintf_buff, d1, d2 , r1, r2);
}

static void test_s()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2; //d1 

	ASSERT_PRINTF("%s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"");
	ASSERT_PRINTF("%.s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"");
	ASSERT_PRINTF("%.0s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"");
	ASSERT_PRINTF("%.1s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"");
	ASSERT_PRINTF("%0s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"");
	ASSERT_PRINTF("%1s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"");
	ASSERT_PRINTF("%s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%.s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%.1s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%.2s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%.3s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%.4s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%.4s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");

	ASSERT_PRINTF("%-s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%-1s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%-2s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%-3s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%-4s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	
	ASSERT_PRINTF("%10s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%	s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%	s	s",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
	ASSERT_PRINTF("%\vs\t\ns",printf_buff, ftprintf_buff, d1, d2 , r1, r2,"abc");
}

static void test_c()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2; //d1 


		for (unsigned char i = 0; i < 126; i++)
		{
			ASSERT_PRINTF("%c",printf_buff, ftprintf_buff, d1, d2 , r1, r2, i);
		}
		ASSERT_PRINTF("%30c",printf_buff, ftprintf_buff, d1, d2 , r1, r2, '#');
	
		ASSERT_PRINTF("%-30c",printf_buff, ftprintf_buff, d1, d2 , r1, r2, '#');
		 
		ASSERT_PRINTF("%#c",printf_buff, ftprintf_buff, d1, d2 , r1, r2, '#');
		ASSERT_PRINTF("%.30c",printf_buff, ftprintf_buff, d1, d2 , r1, r2, '#');
		ASSERT_PRINTF("%-30c",printf_buff, ftprintf_buff, d1, d2 , r1, r2, '#');
		ASSERT_PRINTF("%+30c",printf_buff, ftprintf_buff, d1, d2 , r1, r2, '#');
}

static void test_p()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2; //d1 

	int x = 100;
	ASSERT_PRINTF("%p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (size_t)-1);
	ASSERT_PRINTF("%p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	
	
	ASSERT_PRINTF("%3p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	ASSERT_PRINTF("%0p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	ASSERT_PRINTF("%.30p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	ASSERT_PRINTF("%.0p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	ASSERT_PRINTF("%.1p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	ASSERT_PRINTF("%.2p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	ASSERT_PRINTF("%.3p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, &x);
	ASSERT_PRINTF("%.3p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	
	ASSERT_PRINTF("%03p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%00p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%50p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	
	ASSERT_PRINTF("%0p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%1p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%10p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%11p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);


	ASSERT_PRINTF("%-p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%0-p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%-1p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%-10p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%-11p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	

	ASSERT_PRINTF("%+p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("% p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
	ASSERT_PRINTF("%-100p",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (unsigned) -1);
}


static void test_i()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2; //d1 

	ASSERT_PRINTF("%i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MIN);
	ASSERT_PRINTF("% i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MIN);

	ASSERT_PRINTF("%10.5i", printf_buff, ftprintf_buff, d1, d2 , r1, r2,-216)

	ASSERT_PRINTF("%i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("%012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-+.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%-+100.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);

	ASSERT_PRINTF("% i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("% i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("% 012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% +012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -+.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% -+100.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);

	ASSERT_PRINTF("%+ i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%+ i",printf_buff, ftprintf_buff, d1, d2 , r1, r2,INT_MAX);
	ASSERT_PRINTF("%+ 012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("% +012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -012i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -.12i",printf_buff, ftprintf_buff, d1, d2 , r1, r2, INT_MAX);
	ASSERT_PRINTF("%+ -+100.12i",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);

	ASSERT_PRINTF("%12.10i",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%12.14i",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%14.10i",printf_buff, ftprintf_buff, d1, d2 , r2, r2, INT_MAX);
	
	ASSERT_PRINTF("%-20.31i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%-31.20i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	
	ASSERT_PRINTF("%+-10.0i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+-10.2i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+-10.11i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);

	ASSERT_PRINTF("%+- 10.0i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+- 10.2i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
	ASSERT_PRINTF("%+- 10.11i",printf_buff, ftprintf_buff, d2, d2 , r2, r2, INT_MAX);
}

static void test_x()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2;

	ASSERT_PRINTF("%x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("% x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%#x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 123);
	ASSERT_PRINTF("%x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%#x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

	ASSERT_PRINTF("%+x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.0x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.1x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.2x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.3x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	
	ASSERT_PRINTF("%.0x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.1x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.10x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.11x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%.12x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

	ASSERT_PRINTF("%-x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-0x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-1x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-10x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-11x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-12x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

	ASSERT_PRINTF("%-x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%-0x",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%-1x",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
	ASSERT_PRINTF("%-10x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);


	ASSERT_PRINTF("%-.0x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%-.1x",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%-.2x",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
	ASSERT_PRINTF("%-.3x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);


	ASSERT_PRINTF("%-1.0x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%-2.1x",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
	ASSERT_PRINTF("%-3.2x",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
	ASSERT_PRINTF("%-4.3x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);

	ASSERT_PRINTF("%-1.0x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-2.1x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-10.3x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-11.19x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-19.11x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

	ASSERT_PRINTF("%#x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, LONG_MAX);
	ASSERT_PRINTF("%#x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, LONG_MIN);
}


static void test_X()
{
	char		printf_buff[0xF00];
	char		ftprintf_buff[0xF00];
	long		r1 = 0, r2 = 0;
	int			d1, d2;

	ASSERT_PRINTF("%x",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("% X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%#X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 123);
	ASSERT_PRINTF("%X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%#X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);

	ASSERT_PRINTF("%+X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.0X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.1X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.2X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	ASSERT_PRINTF("%.3X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
	
    ASSERT_PRINTF("%.0X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.1X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.10X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.11X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%.12X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
                       
    ASSERT_PRINTF("%-X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-0X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-1X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-10X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-11X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-12X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
                       
    ASSERT_PRINTF("%-X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
    ASSERT_PRINTF("%-0X",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
    ASSERT_PRINTF("%-1X",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
    ASSERT_PRINTF("%-10X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
                       
                       
    ASSERT_PRINTF("%-.0X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
    ASSERT_PRINTF("%-.1X",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
    ASSERT_PRINTF("%-.2X",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
    ASSERT_PRINTF("%-.3X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
                       
                       
    ASSERT_PRINTF("%-1.0X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
    ASSERT_PRINTF("%-2.1X",printf_buff, ftprintf_buff, d1, d2 , r1, r2,0);
    ASSERT_PRINTF("%-3.2X",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	0);
    ASSERT_PRINTF("%-4.3X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 0);
                       
    ASSERT_PRINTF("%-1.0X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-2.1X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-3.2X",printf_buff, ftprintf_buff, d1, d2 , r1, r2,	UINT_MAX)
    ASSERT_PRINTF("%-4.3X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-10.3X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%-11.19X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
    ASSERT_PRINTF("%-19.11X",printf_buff, ftprintf_buff, d1, d2 , r1, r2, UINT_MAX);
	ASSERT_PRINTF("%#20.19x \n",printf_buff, ftprintf_buff, d1, d2 , r1, r2, -9876);

}

static void test_outrange()
{
	char		printf_buff[314745];
	char		ftprintf_buff[314745];
	long		r1 = 0, r2 = 0;
	int			d1, d2;
	
	ASSERT_PRINTF("%c\n%d    %s",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 'a', INT_MIN, "ASDASGAGAXCVXCVFQQWRGVBJHNKJZXCHIQGIUFDQWHUDHQWIDUHASKJDHABSJKHVCZXCVKJLHGEKIWQEDGHSKJ");

	ASSERT_PRINTF("%p %c\n%d    %s",printf_buff, ftprintf_buff, d1, d2 , r1, r2, 'a',(size_t)-1, INT_MIN, "ASDASGAGAXCVXCVFQQWRGVBJHNKJZXCHIQGIUFDQWHUDHQWIDUHASKJDHABSJKHVCZXCVKJLHGEKIWQEDGHSKJ");

	ASSERT_PRINTF("%p %d\n %i    %s",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (size_t)-1,INT_MIN, INT_MIN,  "ASDASGAGAXCVXCVFQQWRGVBJHNKJZXCHIQGIUFDQWHUDHQWIDUHASKJDHABSJKHVCZXCVKJLHGEKIWQEDGHSKJ");

	ASSERT_PRINTF("%090.196X%0197.108d",printf_buff, ftprintf_buff, d1, d2 , r1, r2,
			1235826471, -853819234)
	ASSERT_PRINTF("\\!/%#22.2x\\!/", printf_buff, ftprintf_buff, d1, d2 , r1, r2, 987654212);

	ASSERT_PRINTF("%u\n\v %p %s %d          asdfsadf  asdfasdf  123",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (size_t)-1, INT_MIN,  "ASDASGAGAXCVXCVFQQWRGVBJHNKJZXCHIQGIUFDQWHUDHQWIDUHASKJDHABSJKHVCZXCVKJLHGEKIWQEDGHSKJ", INT_MIN);


	ASSERT_PRINTF("%u\n\v %p %s %d          asdfsadf  asdfasdf  123",printf_buff, ftprintf_buff, d1, d2 , r1, r2, (size_t)-1, INT_MIN,  "ASDASGAGAXCVXCVFQQWRGVBJHNKJZXCHIQGIUFDQWHUDHQWIDUHASKJDHABSJKHVCZXCVKJLHGEKIWQEDGHSKJ", INT_MIN);
	
	ASSERT_PRINTF("%.3s %040s %.2s   %.2s  %-20u   %012d ",printf_buff, ftprintf_buff, d1, d2 , r1, r2,
			"123213123", "!@#!@#!@#!@#@!#!@#!", "", "\n123\n", UINT_MAX,  INT_MAX);
	ASSERT_PRINTF("hello %",printf_buff, ftprintf_buff, d1, d2 , r1, r2,NULL);
	ASSERT_PRINTF("%",printf_buff, ftprintf_buff, d1, d2 , r1, r2,NULL);


}

#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	SWAP_FD(fd);
	/* [> c s p d i u x X % <] */
	INVOKE_TEST(test_d);
	INVOKE_TEST(test_i);
	INVOKE_TEST(test_u);
	INVOKE_TEST(test_percent);
	INVOKE_TEST(test_s);
	INVOKE_TEST(test_c);
	INVOKE_TEST(test_p);
	INVOKE_TEST(test_x);
	INVOKE_TEST(test_X);
	INVOKE_TEST(test_outrange);
}	
	


