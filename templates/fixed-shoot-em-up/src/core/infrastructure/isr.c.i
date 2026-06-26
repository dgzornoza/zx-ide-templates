
#line 1 "src/core/infrastructure/isr.c"

#line 1 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/z80.h"

 





#line 1 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/stdint.h"

 




#line 1 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/sys/compiler.h"

 





 

 
 
 

 








#line 9 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/stdint.h"



typedef signed char            int8_t;
typedef signed int             int16_t;
typedef signed long            int32_t;

typedef unsigned char          uint8_t;
typedef unsigned int           uint16_t;
typedef unsigned long          uint32_t;

typedef signed char            int_least8_t;
typedef signed int             int_least16_t;
typedef signed long            int_least32_t;

typedef unsigned char          uint_least8_t;
typedef unsigned int           uint_least16_t;
typedef unsigned long          uint_least32_t;

typedef signed int             int_fast8_t;
typedef signed int             int_fast16_t;
typedef signed long            int_fast32_t;

typedef unsigned int           uint_fast8_t;
typedef unsigned int           uint_fast16_t;
typedef unsigned long          uint_fast32_t;




#line 49 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/stdint.h"
typedef long long              int64_t;
typedef unsigned long long     uint64_t;

typedef long long              int_least64_t;
typedef unsigned long long     uint_least64_t;

typedef long long              int_fast64_t;
typedef unsigned long long     uint_fast64_t;




#line 75 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/stdint.h"
typedef int                    intptr_t;


typedef unsigned int           uintptr_t;










typedef long long              intmax_t;
typedef unsigned long long     uintmax_t;































 
 



 
 













#line 11 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/z80.h"

#line 1 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/im2.h"

 











extern void im2_init(void *im2_table_address)  __preserves_regs(b,c,d,e,iyl,iyh);
extern void im2_init_fastcall(void *im2_table_address) __preserves_regs(b,c,d,e,h,l,iyl,iyh) __z88dk_fastcall;




extern void    *im2_install_isr(uint8_t vector,void *isr)    __preserves_regs(b,c,iyl,iyh);
extern void    *im2_install_isr_callee(uint8_t vector,void *isr)    __preserves_regs(b,c,iyl,iyh) __z88dk_callee;



extern void    *im2_create_generic_isr(uint8_t num_callbacks,void *address)    __preserves_regs(iyl,iyh);
extern void    *im2_create_generic_isr_callee(uint8_t num_callbacks,void *address)    __preserves_regs(iyl,iyh) __z88dk_callee;



extern void    *im2_create_generic_isr_8080(uint8_t num_callbacks,void *address)    __preserves_regs(iyl,iyh);
extern void    *im2_create_generic_isr_8080_callee(uint8_t num_callbacks,void *address)    __preserves_regs(iyl,iyh) __z88dk_callee;



extern void    im2_append_generic_callback(uint8_t vector,void *callback)    __preserves_regs(iyl,iyh);
extern void    im2_append_generic_callback_callee(uint8_t vector,void *callback)    __preserves_regs(iyl,iyh) __z88dk_callee;



extern void    im2_prepend_generic_callback(uint8_t vector,void *callback)    __preserves_regs(iyl,iyh);
extern void    im2_prepend_generic_callback_callee(uint8_t vector,void *callback)    __preserves_regs(iyl,iyh) __z88dk_callee;



extern int    im2_remove_generic_callback(uint8_t vector,void *callback)    __preserves_regs(iyl,iyh);
extern int    im2_remove_generic_callback_callee(uint8_t vector,void *callback)    __preserves_regs(iyl,iyh) __z88dk_callee;












#line 12 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/z80.h"







extern void z80_delay_ms(uint16_t ms);
extern void z80_delay_ms_fastcall(uint16_t ms) __z88dk_fastcall;







extern void z80_delay_tstate(uint16_t tstates)  __preserves_regs(d,e);
extern void z80_delay_tstate_fastcall(uint16_t tstates) __preserves_regs(d,e) __z88dk_fastcall;




extern uint8_t    z80_get_int_state(void)    __preserves_regs(b,c,d,e);




extern void z80_set_int_state(uint8_t state)  __preserves_regs(b,c,d,e);
extern void z80_set_int_state_fastcall(uint8_t state) __preserves_regs(a,b,c,d,e,h,l) __z88dk_fastcall;








extern uint8_t z80_inp(uint16_t port)  __preserves_regs(d,e);
extern uint8_t z80_inp_fastcall(uint16_t port) __preserves_regs(d,e) __z88dk_fastcall;




extern void    *z80_inir(void *dst,uint8_t port,uint8_t num)    __preserves_regs(d,e);
extern void    *z80_inir_callee(void *dst,uint8_t port,uint8_t num)    __preserves_regs(d,e) __z88dk_callee;



extern void    *z80_indr(void *dst,uint8_t port,uint8_t num)    __preserves_regs(d,e);
extern void    *z80_indr_callee(void *dst,uint8_t port,uint8_t num)    __preserves_regs(d,e) __z88dk_callee;



extern void    z80_outp(uint16_t port,uint8_t data)    __preserves_regs(d,e);
extern void    z80_outp_callee(uint16_t port,uint8_t data)    __preserves_regs(d,e) __z88dk_callee;



extern void    *z80_otir(void *src,uint8_t port,uint8_t num)    __preserves_regs(d,e);
extern void    *z80_otir_callee(void *src,uint8_t port,uint8_t num)    __preserves_regs(d,e) __z88dk_callee;



extern void    *z80_otdr(void *src,uint8_t port,uint8_t num)    __preserves_regs(d,e);
extern void    *z80_otdr_callee(void *src,uint8_t port,uint8_t num)    __preserves_regs(d,e) __z88dk_callee;











#line 2 "src/core/infrastructure/isr.c"

#line 1 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/string.h"

 




#line 1 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/stddef.h"

 











typedef int           ptrdiff_t;



typedef unsigned int  size_t;


typedef unsigned char max_align_t;



typedef unsigned char wchar_t;






#line 9 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/string.h"












extern int    bcmp(const void *b1,const void *b2,size_t len)   ;
extern int    bcmp_callee(const void *b1,const void *b2,size_t len)    __z88dk_callee;



extern void    bcopy(const void *src,void *dst,size_t len)   ;
extern void    bcopy_callee(const void *src,void *dst,size_t len)    __z88dk_callee;



extern void    bzero(void *mem,size_t n)   ;
extern void    bzero_callee(void *mem,size_t n)    __z88dk_callee;



extern char    *index(const char *s,int c)    __preserves_regs(d,e);
extern char    *index_callee(const char *s,int c)    __preserves_regs(d,e) __z88dk_callee;



extern char    *rindex(const char *s,int c)    __preserves_regs(d);
extern char    *rindex_callee(const char *s,int c)    __preserves_regs(d) __z88dk_callee;



extern char    *strset(char *s,int c)    __preserves_regs(b,c);
extern char    *strset_callee(char *s,int c)    __preserves_regs(b,c) __z88dk_callee;



extern char    *strnset(char *s,int c,size_t n)   ;
extern char    *strnset_callee(char *s,int c,size_t n)    __z88dk_callee;



extern void    *rawmemchr(const void *mem,int c)    __preserves_regs(d,e);
extern void    *rawmemchr_callee(const void *mem,int c)    __preserves_regs(d,e) __z88dk_callee;




extern char    *_memlwr_(void *p,size_t n)    __preserves_regs(d,e);
extern char    *_memlwr__callee(void *p,size_t n)    __preserves_regs(d,e) __z88dk_callee;



extern char    *_memstrcpy_(void *p,const char *s,size_t n)   ;
extern char    *_memstrcpy__callee(void *p,const char *s,size_t n)    __z88dk_callee;



extern char    *_memupr_(void *p,size_t n)    __preserves_regs(d,e);
extern char    *_memupr__callee(void *p,size_t n)    __preserves_regs(d,e) __z88dk_callee;



extern char    *_strrstrip_(char *s)   ;
extern char    *_strrstrip__fastcall(char *s) __z88dk_fastcall;






extern int ffs(int i)  __preserves_regs(b,c,d,e);
extern int ffs_fastcall(int i) __preserves_regs(b,c,d,e) __z88dk_fastcall;







extern int ffsl(long i)  __preserves_regs(b,c);
extern int ffsl_fastcall(long i) __preserves_regs(b,c,d,e) __z88dk_fastcall;




extern void    *memccpy(void *dst,const void *src,int c,size_t n)   ;
extern void    *memccpy_callee(void *dst,const void *src,int c,size_t n)    __z88dk_callee;



extern void    *memchr(const void *s,int c,size_t n)   ;
extern void    *memchr_callee(const void *s,int c,size_t n)    __z88dk_callee;



extern int    memcmp(const void *s1,const void *s2,size_t n)   ;
extern int    memcmp_callee(const void *s1,const void *s2,size_t n)    __z88dk_callee;



extern void    *memcpy(void *dst,const void *src,size_t n)   ;
extern void    *memcpy_callee(void *dst,const void *src,size_t n)    __z88dk_callee;



extern void    *memmem(const void *haystack,size_t haystack_len,const void *needle,size_t needle_len)   ;
extern void    *memmem_callee(const void *haystack,size_t haystack_len,const void *needle,size_t needle_len)    __z88dk_callee;



extern void    *memmove(void *dst,const void *src,size_t n)   ;
extern void    *memmove_callee(void *dst,const void *src,size_t n)    __z88dk_callee;



extern void    *memrchr(const void *s,int c,size_t n)   ;
extern void    *memrchr_callee(const void *s,int c,size_t n)    __z88dk_callee;



extern void    *memset(void *s,int c,size_t n)   ;
extern void    *memset_callee(void *s,int c,size_t n)    __z88dk_callee;



extern void    *memset_wr(volatile void *s,int c,size_t n)   ;
extern void    *memset_wr_callee(volatile void *s,int c,size_t n)    __z88dk_callee;



extern void    *memswap(void *s1,void *s2,size_t n)   ;
extern void    *memswap_callee(void *s1,void *s2,size_t n)    __z88dk_callee;



extern char    *stpcpy(char *dst,const char *src)   ;
extern char    *stpcpy_callee(char *dst,const char *src)    __z88dk_callee;



extern char    *stpncpy(char *dst,const char *src,size_t n)   ;
extern char    *stpncpy_callee(char *dst,const char *src,size_t n)    __z88dk_callee;



extern int    strcasecmp(const char *s1,const char *s2)    __preserves_regs(b);
extern int    strcasecmp_callee(const char *s1,const char *s2)    __preserves_regs(b) __z88dk_callee;



extern char    *strcat(char *dst,const char *src)   ;
extern char    *strcat_callee(char *dst,const char *src)    __z88dk_callee;



extern char    *strchr(const char *s,int c)    __preserves_regs(d,e);
extern char    *strchr_callee(const char *s,int c)    __preserves_regs(d,e) __z88dk_callee;



extern char    *strchrnul(const char *s,int c)    __preserves_regs(d,e);
extern char    *strchrnul_callee(const char *s,int c)    __preserves_regs(d,e) __z88dk_callee;



extern int    strcmp(const char *s1,const char *s2)   ;
extern int    strcmp_callee(const char *s1,const char *s2)    __z88dk_callee;



extern int    strcoll(const char *s1,const char *s2)   ;
extern int    strcoll_callee(const char *s1,const char *s2)    __z88dk_callee;



extern char    *strcpy(char *dst,const char *src)   ;
extern char    *strcpy_callee(char *dst,const char *src)    __z88dk_callee;



extern size_t    strcspn(const char *s,const char *nspn)   ;
extern size_t    strcspn_callee(const char *s,const char *nspn)    __z88dk_callee;



extern char    *strdup(const char *s)   ;
extern char    *strdup_fastcall(const char *s) __z88dk_fastcall;



extern char    *strerror(int errnum)    __preserves_regs(d);
extern char    *strerror_fastcall(int errnum) __preserves_regs(d) __z88dk_fastcall;



extern int    stricmp(const char *s1,const char *s2)    __preserves_regs(b);
extern int    stricmp_callee(const char *s1,const char *s2)    __preserves_regs(b) __z88dk_callee;



extern size_t    strlcat(char *dst,const char *src,size_t n)   ;
extern size_t    strlcat_callee(char *dst,const char *src,size_t n)    __z88dk_callee;



extern size_t    strlcpy(char *dst,const char *src,size_t n)   ;
extern size_t    strlcpy_callee(char *dst,const char *src,size_t n)    __z88dk_callee;



extern size_t    strlen(const char *s)    __preserves_regs(d,e);
extern size_t    strlen_fastcall(const char *s) __preserves_regs(d,e) __z88dk_fastcall;



extern char    *strlwr(char *s)    __preserves_regs(b,c,d,e);
extern char    *strlwr_fastcall(char *s) __preserves_regs(b,c,d,e,h,l) __z88dk_fastcall;



extern int    strncasecmp(const char *s1,const char *s2,size_t n)   ;
extern int    strncasecmp_callee(const char *s1,const char *s2,size_t n)    __z88dk_callee;



extern char    *strncat(char *dst,const char *src,size_t n)   ;
extern char    *strncat_callee(char *dst,const char *src,size_t n)    __z88dk_callee;



extern char    *strnchr(const char *s,size_t n,int c)   ;
extern char    *strnchr_callee(const char *s,size_t n,int c)    __z88dk_callee;



extern int    strncmp(const char *s1,const char *s2,size_t n)   ;
extern int    strncmp_callee(const char *s1,const char *s2,size_t n)    __z88dk_callee;



extern char    *strncpy(char *dst,const char *src,size_t n)   ;
extern char    *strncpy_callee(char *dst,const char *src,size_t n)    __z88dk_callee;



extern char    *strndup(const char *s,size_t n)   ;
extern char    *strndup_callee(const char *s,size_t n)    __z88dk_callee;



extern int    strnicmp(const char *s1,const char *s2,size_t n)   ;
extern int    strnicmp_callee(const char *s1,const char *s2,size_t n)    __z88dk_callee;



extern size_t    strnlen(const char *s,size_t max_len)   ;
extern size_t    strnlen_callee(const char *s,size_t max_len)    __z88dk_callee;



extern char    *strpbrk(const char *s,const char *set)    __preserves_regs(b);
extern char    *strpbrk_callee(const char *s,const char *set)    __preserves_regs(b) __z88dk_callee;



extern char    *strrchr(const char *s,int c)    __preserves_regs(d);
extern char    *strrchr_callee(const char *s,int c)    __preserves_regs(d) __z88dk_callee;



extern size_t    strrcspn(const char *s,const char *set)   ;
extern size_t    strrcspn_callee(const char *s,const char *set)    __z88dk_callee;



extern char    *strrev(char *s)   ;
extern char    *strrev_fastcall(char *s) __preserves_regs(h,l) __z88dk_fastcall;



extern size_t    strrspn(const char *s,const char *set)   ;
extern size_t    strrspn_callee(const char *s,const char *set)    __z88dk_callee;



extern char    *strrstr(const char *s,const char *subs)   ;
extern char    *strrstr_callee(const char *s,const char *subs)    __z88dk_callee;



extern char    *strrstrip(char *s)   ;
extern char    *strrstrip_fastcall(char *s) __preserves_regs(h,l) __z88dk_fastcall;



extern char    *strsep(char **s,const char *delim)   ;
extern char    *strsep_callee(char **s,const char *delim)    __z88dk_callee;



extern size_t    strspn(const char *s,const char *pfx)   ;
extern size_t    strspn_callee(const char *s,const char *pfx)    __z88dk_callee;



extern char    *strstr(const char *s,const char *subs)    __preserves_regs(b,c);
extern char    *strstr_callee(const char *s,const char *subs)    __preserves_regs(b,c) __z88dk_callee;






extern char *strstrip(char *s)  __preserves_regs(b,c,d,e);
extern char *strstrip_fastcall(char *s) __preserves_regs(b,c,d,e) __z88dk_fastcall;




extern char    *strtok(char *s,const char *delim)   ;
extern char    *strtok_callee(char *s,const char *delim)    __z88dk_callee;



extern char    *strtok_r(char *s,const char *delim,char **last_s)   ;
extern char    *strtok_r_callee(char *s,const char *delim,char **last_s)    __z88dk_callee;



extern char    *strupr(char *s)    __preserves_regs(b,c,d,e);
extern char    *strupr_fastcall(char *s) __preserves_regs(b,c,d,e,h,l) __z88dk_fastcall;



extern size_t    strxfrm(char *dst,const char *src,size_t n)   ;
extern size_t    strxfrm_callee(char *dst,const char *src,size_t n)    __z88dk_callee;







#line 393 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/string.h"
   extern int ffsll(long long i) __preserves_regs(b,c);
   extern int ffsll_callee(long long i) __preserves_regs(b,c) __z88dk_callee;
   

   

   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   

   


#line 3 "src/core/infrastructure/isr.c"

#line 1 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/intrinsic.h"

 





 





#pragma disable_warning 84


#pragma disable_warning 112







#line 34 "/opt/z88dk/lib/config//../..//include/_DEVELOPMENT/common/intrinsic.h"
extern void intrinsic_ldi(void*,void*) __z88dk_callee;



extern void intrinsic_outi(void*) __z88dk_fastcall;



extern void intrinsic_ini(void*) __z88dk_fastcall;





extern void    intrinsic_stub(void)    __preserves_regs(a,b,c,d,e,h,l);


extern void    intrinsic_di(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_ei(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_halt(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_reti(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_retn(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_im_0(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_im_1(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_im_2(void)    __preserves_regs(a,b,c,d,e,h,l);

extern void    intrinsic_nop(void)    __preserves_regs(a,b,c,d,e,h,l);


extern void    intrinsic_ex_de_hl(void)    __preserves_regs(a,b,c);

extern void    intrinsic_exx(void)    __preserves_regs(a);


extern void    *intrinsic_return_bc(void)    __preserves_regs(a,b,c,d,e);

extern void    *intrinsic_return_de(void)    __preserves_regs(a,b,c,d,e);





extern unsigned int intrinsic_swap_endian_16(unsigned int n)  __preserves_regs(b,c,d,e);
extern unsigned int intrinsic_swap_endian_16_fastcall(unsigned int n) __preserves_regs(b,c,d,e) __z88dk_fastcall;







extern unsigned long intrinsic_swap_endian_32(unsigned long n)  __preserves_regs(b,c);
extern unsigned long intrinsic_swap_endian_32_fastcall(unsigned long n) __preserves_regs(b,c) __z88dk_fastcall;







extern unsigned long intrinsic_swap_word_32(unsigned long n)  __preserves_regs(a,b,c);
extern unsigned long intrinsic_swap_word_32_fastcall(unsigned long n) __preserves_regs(a,b,c) __z88dk_fastcall;









#line 5 "src/core/infrastructure/isr.c"



volatile uint8_t frame_tick = 0;

 
 
 
 
 
 
 



 





 void isr (void) __naked { __asm__("\tEXTERN\tasm_im2_push_registers\n" "\tEXTERN\tasm_im2_pop_registers\n" "\n" "\tcall\tasm_im2_push_registers\n" "\tcall\t__im2_isr_" "isr" "\n" "\tcall\tasm_im2_pop_registers\n" "\n" "\tei\n" "\treti\n" ); } void _im2_isr_isr (void) 
{
    frame_tick++;
}

 
void im2_setup(void)
{
     __builtin_memset( ((void *)( ((unsigned int)0xD0)  * ((unsigned int)256) )) , ((unsigned int)0xD1) , 257 ) ;

     (*(unsigned char *)( ((unsigned char *)((unsigned int)( ((unsigned int)0xD1)  * ((unsigned int)256) ) + ((unsigned int)0xD1) )) ) = 195 ) ;
     (*(unsigned int *)( ((unsigned char *)((unsigned int)( ((unsigned int)0xD1)  * ((unsigned int)256) ) + ((unsigned int)0xD1) ))  + 1 ) = (unsigned int)isr ) ;

     im2_init_fastcall( ((void *)( ((unsigned int)0xD0)  * ((unsigned int)256) )) ) ;

    intrinsic_ei();
}

