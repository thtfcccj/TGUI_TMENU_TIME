/*
           pyInput.c - Æ´ÒôÊäÈë·¨Ä£¿é

           Ô­×÷:ÕÅ ¿­
           ¸ÄĞ´:Àî Ç¿(mail2li@21cn.com)
           ÖØ¸ÄĞ´£ºdap_ccj
           ĞŞÕı²Î¿¼£ºhttps://blog.csdn.net/zerokkqq/article/details/6413694
*/

#include <string.h>
#include <stdio.h>

#include "PinYinIME.h"

/* "Æ´ÒôÊäÈë·¨ºº×ÖÅÅÁĞ±í,Âë±í(mb)" */
static const unsigned char Pymb_a[]     ={"°¡°¢ß¹àÄëçï¹"};
static const unsigned char Pymb_ai[]    ={"°£°¤°¥°¦°§°¨°©°ª°«°¬°­°®°¯ŞßàÈàÉæÈè¨êÓíÁïÍö°"};
static const unsigned char Pymb_an[]    ={"°°°±°²°³°´°µ°¶°·°¸ÚÏÛûŞîáíâÖèñï§ğÆ÷ö"};
static const unsigned char Pymb_ang[]   ={"°¹°º°»"};
static const unsigned char Pymb_ao[]    ={"°¼°½°¾°¿°À°Á°Â°Ã°ÄÛêŞÖà»á®âÚåÛæÁæñéáñúòüöË÷¡÷é"};

static const unsigned char Pymb_b[]     ={"°É°Ë²»±¾±È°Ñ°×"};
static const unsigned char Pymb_ba[]    ={"°Å°Æ°Ç°È°É°Ê°Ë°Ì°Í°Î°Ï°Ğ°Ñ°Ò°Ó°Ô°Õ°ÖÜØİÃá±å±îÙôÎöÑ÷É"};
static const unsigned char Pymb_bai[]   ={"°×°Ø°Ù°Ú°Û°Ü°İ°Ş²®Şãêş"};
static const unsigned char Pymb_ban[]   ={"°ß°à°á°â°ã°ä°å°æ°ç°è°é°ê°ë°ì°íÚæÛàêÚîÓñ£ñ­ô²"};
static const unsigned char Pymb_bang[]  ={"°î°ï°ğ°ñ°ò°ó°ô°õ°ö°÷°ø°ùİòäº"};
static const unsigned char Pymb_bao[]   ={"°ú°û°ü°ı°ş±¡±¢±£±¤±¥±¦±§±¨±©±ª±«±¬ÅÙÅÚÙèİáæßìÒğ±ñÙõÀöµ"};
static const unsigned char Pymb_bei[]   ={"±­±®±¯±°±±±²±³±´±µ±¶±·±¸±¹±º±»±ÛØÃÚéÚıÛıİÉİíßÂã£íÕğÇñØöÍ÷¹"};
static const unsigned char Pymb_ben[]   ={"±¼±½±¾±¿ÛÎÛĞêÚï¼"};
static const unsigned char Pymb_beng[]  ={"°ö±À±Á±Â±Ã±Ä±ÅÜ¡àÔê´"};
static const unsigned char Pymb_bi[]    ={"±Æ±Ç±È±É±Ê±Ë±Ì±Í±Î±Ï±Ğ±Ñ±Ò±Ó±Ô±Õ±Ö±×±Ø±Ù±Ú±Û±Ü±İÃØÃÚØ°ÙÂÛıÜÅÜêİ©İÉŞµßÁßÙáùâØã¹ääå¨åöåşæ¾æÔèµêÚî¯îéïõñÔóÙóëó÷ô°ôÅõÏ÷Â"};
static const unsigned char Pymb_bian[]  ={"±Ş±ß±à±á±â±ã±ä±å±æ±ç±è±éØÒÛÍÜĞâíãêçÂìÔí¾íÜñ¹ñÛòùóÖöı"};
static const unsigned char Pymb_biao[]  ={"±ê±ë±ì±íæ»æôè¼ì©ì­ì®ìáïÚïğñ¦ñÑ÷§÷Ô"};
static const unsigned char Pymb_bie[]   ={"±î±ï±ğ±ñõ¿"};
static const unsigned char Pymb_bin[]   ={"±ò±ó±ô±õ±ö±÷ÙÏáÙçÍçãéÄéëë÷ïÙ÷Æ÷Ş"};
static const unsigned char Pymb_bing[]  ={"±ø±ù±ú±û±ü±ı±ş²¡²¢ÆÁÙ÷ÙûÚûŞğéÄ"};
static const unsigned char Pymb_bo[]    ={"°Ø°Ù°ş±¡²£²¤²¥²¦²§²¨²©²ª²«²¬²­²®²¯²°²±²²²³²´²µ²·ØÃÙñŞ¬à£âÄéŞë¢íçîàğ¾ô¤õÀõËõÛ"};
static const unsigned char Pymb_bu[]    ={"±¤²¶²·²¸²¹²º²»²¼²½²¾²¿²ÀÆÒß²åÍê³êÎîĞîßõ³"};

static const unsigned char Pymb_c[]     ={"²Å´Î³¤³ö²à´Ó"};
static const unsigned char Pymb_ca[]    ={"²Áàêíå"};
static const unsigned char Pymb_cai[]   ={"²Â²Ã²Ä²Å²Æ²Ç²È²É²Ê²Ë²Ì"};
static const unsigned char Pymb_can[]   ={"²Í²Î²Ï²Ğ²Ñ²Ò²Óåîæîè²ôÓ÷õ"};
static const unsigned char Pymb_cang[]  ={"²Ô²Õ²Ö²×²ØØ÷"};
static const unsigned char Pymb_cao[]   ={"²Ù²Ú²Û²Ü²İÜ³àĞäîó©ô½"};
static const unsigned char Pymb_ce[]    ={"²Ş²ß²à²á²ââü"};
static const unsigned char Pymb_cen[]   ={"²Îá¯ä¹"};
static const unsigned char Pymb_ceng[]  ={"²ã²äÔøàá"};
static const unsigned char Pymb_cha[]   ={"²å²æ²ç²è²é²ê²ë²ì²í²î²ïÉ²ÔûàêâªâÇãâæ±è¾é«é¶éßïÊïïñÃ"};
static const unsigned char Pymb_chai[]  ={"²î²ğ²ñ²òÙ­îÎğûò²ö·"};
static const unsigned char Pymb_chan[]  ={"²ó²ô²õ²ö²÷²ø²ù²ú²û²üµ¥ÙæÚÆİÛâÜâãäıå¤åîåñæ¿êèìøïâó¸õğ"};
static const unsigned char Pymb_chang[] ={"²ı²ş³¡³¢³£³¤³¥³¦³§³¨³©³ª³«ØöÛËÜÉİÅáäâêã®ãÑæ½æÏêÆë©öğ"};
static const unsigned char Pymb_chao[]  ={"³¬³­³®³¯³°³±³²³³³´´Â½Ëâ÷êËñé"};
static const unsigned char Pymb_che[]   ={"³µ³¶³·³¸³¹³ºÛååøíº"};
static const unsigned char Pymb_chen[]  ={"³»³¼³½³¾³¿³À³Á³Â³Ã³Ä³ÆÚÈÚßŞÓàÁå·è¡é´í×ö³"};
static const unsigned char Pymb_cheng[] ={"³Å³Æ³Ç³È³É³Ê³Ë³Ì³Í³Î³Ï³Ğ³Ñ³Ò³ÓÊ¢Ø©ÛôàááçèÇèßëóîªîñîõñÎòÉõ¨"};
static const unsigned char Pymb_chi[]   ={"³Ô³Õ³Ö³×³Ø³Ù³Ú³Û³Ü³İ³Ş³ß³à³á³â³ãÙÑÛæÜ¯Üİß³ßêà´àÍáÜâÁæÊë·í÷ğ·ñ¡ñİò¿ó¤ó×óøôùõØ÷Î"};
static const unsigned char Pymb_chong[] ={"³ä³å³æ³ç³èÖÖÖØÜûâçã¿ï¥ô©ô¾"};
static const unsigned char Pymb_chou[]  ={"³é³ê³ë³ì³í³î³ï³ğ³ñ³ò³ó³ôÙ±àüã°ñ¬öÅ"};
static const unsigned char Pymb_chu[]   ={"³õ³ö³÷³ø³ù³ú³û³ü³ı³ş´¡´¢´£´¤´¥´¦ĞóØ¡Û»âğãÀç©èÆèúéËñÒòÜõé÷í"};
static const unsigned char Pymb_chuai[] ={"´§ŞõàÜëúõß"};
static const unsigned char Pymb_chuan[] ={"´¨´©´ª´«´¬´­´®â¶å×çİë°îËô­"};
static const unsigned char Pymb_chuang[]={"´¯´°´±´²´³´´âëğÚ"};
static const unsigned char Pymb_chui[]  ={"´µ´¶´·´¸´¹Úïé¢é³"};
static const unsigned char Pymb_chun[]  ={"´º´»´¼´½´¾´¿´Àİ»ğÈòí"};
static const unsigned char Pymb_chuo[]  ={"´Á´Âà¨åÁê¡õÖöº"};
static const unsigned char Pymb_ci[]    ={"²î´Ã´Ä´Å´Æ´Ç´È´É´Ê´Ë´Ì´Í´ÎËÅ×ÈÜëßÚìôğËôÒôÙ"};
static const unsigned char Pymb_cong[]  ={"´Ï´Ğ´Ñ´Ò´Ó´ÔÜÊäÈæõçıè®èÈ"};
static const unsigned char Pymb_cou[]   ={"´Õé¨ê£ëí"};
static const unsigned char Pymb_cu[]    ={"´Ö´×´Ø´ÙİıáŞâ§éãõ¡õ¾õí"};
static const unsigned char Pymb_cuan[]  ={"´Ú´Û´ÜÔÜÙàß¥ìàïé"};
static const unsigned char Pymb_cui[]   ={"´İ´Ş´ß´à´á´â´ã´äİÍßıã²è­éÁë¥ö¿"};
static const unsigned char Pymb_cun[]   ={"´å´æ´çââñå"};
static const unsigned char Pymb_cuo[]   ={"´è´é´ê´ë´ì´íØÈáÏëâï±ïóğîğûõºõãõò"};

static const unsigned char Pymb_d[]     ={"µÄµãµÈ¶àµ½´ó"};
static const unsigned char Pymb_da[]    ={"´î´ï´ğ´ñ´ò´óŞÇßÕàªâòæ§í³ğãñ×óÎ÷°÷²"};
static const unsigned char Pymb_dai[]   ={"´ó´ô´õ´ö´÷´ø´ù´ú´û´ü´ı´şµ¡Ü¤ß°ß¾á·åÊææçªçé÷ì"};
static const unsigned char Pymb_dan[]   ={"µ¢µ£µ¤µ¥µ¦µ§µ¨µ©µªµ«µ¬µ­µ®µ¯µ°ÉÄÊ¯ÙÙİÌà¢å£ééêæíñğãğ÷ñõóì"};
static const unsigned char Pymb_dang[]  ={"µ±µ²µ³µ´µµÚÔÛÊİĞå´í¸îõñÉ"};
static const unsigned char Pymb_dao[]   ={"µ¶µ·µ¸µ¹µºµ»µ¼µ½µ¾µ¿µÀµÁØÖß¶àüâáë®ìâôî"};
static const unsigned char Pymb_de[]    ={"µÂµÃµÄµØï½"};
static const unsigned char Pymb_deng[]  ={"³ÎµÅµÆµÇµÈµÉµÊµËàâáØê­íãïëô£"};
static const unsigned char Pymb_di[]    ={"µÄµÌµÍµÎµÏµĞµÑµÒµÓµÔµÕµÖµ×µØµÙµÚµÛµÜµİµŞÌáØµÙáÚ®ÚĞÛ¡Ûæİ¶àÖæ·èÜé¦êëíÆíÚíûïáôÆ÷¾"};
static const unsigned char Pymb_dia[]   ={"àÇ"};
static const unsigned char Pymb_dian[]  ={"µßµàµáµâµãµäµåµæµçµèµéµêµëµìµíµîÚçÛãáÛçèîäñ°ñ²ô¡õÚ"};
static const unsigned char Pymb_diao[]  ={"µïµğµñµòµóµôµõµöµ÷îöï¢õõöô"};
static const unsigned char Pymb_die[]   ={"µøµùµúµûµüµıµşÛìÜ¦Şéà©ëºğ¬ñóõŞöø"};
static const unsigned char Pymb_ding[]  ={"¶¡¶¢¶£¶¤¶¥¶¦¶§¶¨¶©Øêà¤çàëëíÖî®îúğÛñôôú"};
static const unsigned char Pymb_diu[]   ={"¶ªîû"};
static const unsigned char Pymb_dong[]  ={"¶«¶¬¶­¶®¶¯¶°¶±¶²¶³¶´ßËá´á¼ë±ëËëØíÏğ´"};
static const unsigned char Pymb_dou[]   ={"¶µ¶¶¶·¶¸¶¹¶º¶»¶¼İúñ¼ò½óû"};
static const unsigned char Pymb_du[]    ={"¶¼¶½¶¾¶¿¶À¶Á¶Â¶Ã¶Ä¶Å¶Æ¶Ç¶È¶É¶ÊÜ¶à½äÂèüë¹ó¼óÆ÷Ç÷ò"};
static const unsigned char Pymb_duan[]  ={"¶Ë¶Ì¶Í¶Î¶Ï¶Ğé²ìÑóı"};
static const unsigned char Pymb_dui[]   ={"¶Ñ¶Ò¶Ó¶Ôí¡í­íÔ"};
static const unsigned char Pymb_dun[]   ={"¶Õ¶Ö¶×¶Ø¶Ù¶Ú¶Û¶Ü¶İãçìÀí»íâíïïæõ»"};
static const unsigned char Pymb_duo[]   ={"¶È¶Ş¶ß¶à¶á¶â¶ã¶ä¶å¶æ¶ç¶è¶éÍÔßÍßáãõç¶îìñÖõâ"};

static const unsigned char Pymb_e[]     ={"°¢¶ê¶ë¶ì¶í¶î¶ï¶ğ¶ñ¶ò¶ó¶ô¶õ¶öÅ¶Ø¬ÚÌÛÑÜÃİ­İàßÀãµåíæ¹éîëñï°ïÉğÊò¦öù"};
static const unsigned char Pymb_ei[]    ={"ÚÀ"};
static const unsigned char Pymb_en[]    ={"¶÷àÅİìŞô"};
static const unsigned char Pymb_er[]    ={"¶ø¶ù¶ú¶û¶ü¶ı¶ş·¡åÇçíîïğ¹öÜ"};

static const unsigned char Pymb_f[]     ={"·É·Ö·¢·ç·¨·½"};
static const unsigned char Pymb_fa[]    ={"·¢·£·¤·¥·¦·§·¨·©ÛÒíÀ"};
static const unsigned char Pymb_fan[]   ={"·ª·«·¬·­·®·¯·°·±·²·³·´·µ·¶···¸·¹·ºŞ¬ŞÀá¦èóìÜî²õì"};
static const unsigned char Pymb_fang[]  ={"·»·¼·½·¾·¿·À·Á·Â·Ã·Ä·ÅØÎÚúèÊîÕô³öĞ"};
static const unsigned char Pymb_fei[]   ={"·Æ·Ç·È·É·Ê·Ë·Ì·Í·Î·Ï·Ğ·ÑÜÀáôã­äÇåúç³é¼êÚëèì³ìéíÉïĞğòòãóõôäö­öî"};
static const unsigned char Pymb_fen[]   ={"·Ò·Ó·Ô·Õ·Ö·×·Ø·Ù·Ú·Û·Ü·İ·Ş·ß·àÙÇå¯çãèûêÚö÷÷÷"};
static const unsigned char Pymb_feng[]  ={"·á·â·ã·ä·å·æ·ç·è·é·ê·ë·ì·í·î·ïÙºÛºİ×ßôããí¿"};
static const unsigned char Pymb_fo[]    ={"·ğ"};
static const unsigned char Pymb_fou[]   ={"·ñó¾"};
static const unsigned char Pymb_fu[]    ={"·ğ·ò·ó·ô·õ·ö·÷·ø·ù·ú·û·ü·ı·ş¸¡¸¢¸£¸¤¸¥¸¦¸§¸¨¸©¸ª¸«¸¬¸­¸®¸¯¸°¸±¸²¸³¸´¸µ¸¶¸·¸¸¸¹¸º¸»¸¼¸½¸¾¸¿¸ÀÙëÙìÚâÛ®Ü½ÜÀÜŞÜòİ³İÊŞÔß»á¥âöäæåõæÚæâç¦ç¨èõêçìğíÉíêíëî·ïûğ¥ò¶òİòğòóôïõÃõÆöÖöû"};

static const unsigned char Pymb_g[]     ={"¸ö¸ù¸÷¸ß¹ı¸ø"};
static const unsigned char Pymb_ga[]    ={"¸Á¸Â¼Ğ¿§Ù¤ŞÎæØæÙê¸îÅ"};
static const unsigned char Pymb_gai[]   ={"¸Ã¸Ä¸Å¸Æ¸Ç¸È½æØ¤ÚëÛòê®êàëÜ"};
static const unsigned char Pymb_gan[]   ={"¸É¸Ê¸Ë¸Ì¸Í¸Î¸Ï¸Ğ¸Ñ¸Ò¸ÓÛáÜÕŞÏß¦ãïäÆä÷ç¤éÏêºí·ğáôû"};
static const unsigned char Pymb_gang[]  ={"¸Ô¸Õ¸Ö¸×¸Ø¸Ù¸Ú¸Û¸Ü¿¸í°î¸óà"};
static const unsigned char Pymb_gao[]   ={"¸İ¸Ş¸ß¸à¸á¸â¸ã¸ä¸å¸æØºÚ¾Û¬Ş»çÉéÀéÂê½ï¯"};
static const unsigned char Pymb_ge[]    ={"¸Ç¸ç¸è¸é¸ê¸ë¸ì¸í¸î¸ï¸ğ¸ñ¸ò¸ó¸ô¸õ¸ö¸÷ºÏ¿©ØªØîÛÁÛÙÜªàÃæüë¡ëõîşïÓñËò´ô´÷À"};
static const unsigned char Pymb_gei[]   ={"¸ø"};
static const unsigned char Pymb_gen[]   ={"¸ù¸úØ¨İ¢ßçôŞ"};
static const unsigned char Pymb_geng[]  ={"¸û¸ü¸ı¸ş¹¡¹¢¹£¾±ßìâÙç®öá"};
static const unsigned char Pymb_gong[]  ={"¹¤¹¥¹¦¹§¹¨¹©¹ª¹«¹¬¹­¹®¹¯¹°¹±¹²ŞÃçîëÅò¼ö¡"};
static const unsigned char Pymb_gou[]   ={"¹³¹´¹µ¹¶¹·¹¸¹¹¹º¹»ØşÚ¸á¸åÜæÅçÃèÛêíì°óÑóô÷¸"};
static const unsigned char Pymb_gu[]    ={"¹¼¹½¹¾¹¿¹À¹Á¹Â¹Ã¹Ä¹Å¹Æ¹Ç¹È¹É¹Ê¹Ë¹Ì¹Í¼ÖØÅÚ¬İÔáÄãéèôéïêôêöëûì±î­î¹îÜïÀğ³ğÀğóòÁôşõıöñ÷½"};
static const unsigned char Pymb_gua[]   ={"¹Î¹Ï¹Ğ¹Ñ¹Ò¹ÓØÔÚ´ßÉèéëÒğ»"};
static const unsigned char Pymb_guai[]  ={"¹Ô¹Õ¹ÖŞâ"};
static const unsigned char Pymb_guan[]  ={"¹×¹Ø¹Ù¹Ú¹Û¹Ü¹İ¹Ş¹ß¹à¹áÂÚÙÄİ¸ŞèäÊîÂğÙ÷¤"};
static const unsigned char Pymb_guang[] ={"¹â¹ã¹äßÛáîèæë×"};
static const unsigned char Pymb_gui[]   ={"¹å¹æ¹ç¹è¹é¹ê¹ë¹ì¹í¹î¹ï¹ğ¹ñ¹ò¹ó¹ôÈ²ØĞØÛâÑå³æ£èíêÁêĞğ§óşöÙ÷¬"};
static const unsigned char Pymb_gun[]   ={"¹õ¹ö¹÷ÙòçµíŞöç"};
static const unsigned char Pymb_guo[]   ={"¹ø¹ù¹ú¹û¹ü¹ıÎĞÙåÛöŞâßÃàşáÆâ£é¤ë½ï¾ñøòäòå"};

static const unsigned char Pymb_h[]     ={"ºÃºÍºó»¹»áºÜ"};
static const unsigned char Pymb_ha[]    ={"¸ò¹şîş"};
static const unsigned char Pymb_hai[]   ={"º¡º¢º£º¤º¥º¦º§»¹¿ÈàËëÜõ°"};
static const unsigned char Pymb_han[]   ={"º¨º©ºªº«º¬º­º®º¯º°º±º²º³º´ºµº¶º·º¸º¹ººÚõİÕŞşå«êÏìÊñüò¥òÀ÷ı"};
static const unsigned char Pymb_hang[]  ={"º»º¼º½¿ÔÏïĞĞãìç¬ñş"};
static const unsigned char Pymb_hao[]   ={"¸äº¾º¿ºÀºÁºÂºÃºÄºÅºÆºÑİïŞ¶àÆàãå©å°ê»ğ©ò«òº"};
static const unsigned char Pymb_he[]    ={"ºÇºÈºÉºÊºËºÌºÍºÎºÏºĞºÑºÒºÓºÔºÕºÖº×ºØÏÅÚ­ÛÀÛÖàÀãØæüêÂîÁò¢òÂôç"};
static const unsigned char Pymb_hei[]   ={"ºÙºÚ"};
static const unsigned char Pymb_hen[]   ={"ºÛºÜºİºŞ"};
static const unsigned char Pymb_heng[]  ={"ºßºàºáºâºãŞ¿çñèì"};
static const unsigned char Pymb_hong[]  ={"ºäºåºæºçºèºéºêºëºìÙäÙêÚ§İ¦Ş®Ş°ãÈãü"};
static const unsigned char Pymb_hou[]   ={"ºíºîºïºğºñºòºóÜ©ááåËğúóóô×ö×÷¿"};
static const unsigned char Pymb_hu[]    ={"ºËºôºõºöº÷ºøºùºúºûºüºıºş»¡»¢»£»¤»¥»¦»§Ùüßüàñá²â©âïã±ä°äïçúéÎéõì²ìÃìÎìæìèìïğ­ğÀğÉğ×ò®óËõ­õú÷½"};
static const unsigned char Pymb_hua[]   ={"»¨»©»ª»«»¬»­»®»¯»°æèèëí¹îü"};
static const unsigned char Pymb_huai[]  ={"»±»²»³»´»µõ×"};
static const unsigned char Pymb_huan[]  ={"»¶»·»¸»¹»º»»»¼»½»¾»¿»À»Á»Â»ÃÛ¨Û¼ÛùİÈß§à÷âµä¡ä½äñå¾åÕçÙïÌöé÷ß"};
static const unsigned char Pymb_huang[] ={"»Ä»Å»Æ»Ç»È»É»Ê»Ë»Ì»Í»Î»Ï»Ğ»ÑÚòáåäÒäêåØè«ëÁñ¥ó¨óòöü"};
static const unsigned char Pymb_hui[]   ={"»Ò»Ó»Ô»Õ»Ö»×»Ø»Ù»Ú»Û»Ü»İ»Ş»ß»à»á»â»ã»ä»å»æÀ£Ú¶ÜîÜöŞ¥ßÔßÜà¹ãÄä§ä«åççÀèíêÍí£ò³ó³÷â"};
static const unsigned char Pymb_hun[]   ={"»ç»è»é»ê»ë»ìÚ»âÆãÔäãçõ"};
static const unsigned char Pymb_huo[]   ={"ºÍ»í»î»ï»ğ»ñ»ò»ó»ô»õ»öØåŞ½ß«àëâ·îØïÁïìñëó¶"};

static const unsigned char Pymb_j[]     ={"¾Í½ø½«¼¸¾Å¼Ó"};
static const unsigned char Pymb_ji[]    ={"¸ø»÷»ø»ù»ú»û»ü»ı»ş¼¡¼¢¼£¼¤¼¥¼¦¼§¼¨¼©¼ª¼«¼¬¼­¼®¼¯¼°¼±¼²¼³¼´¼µ¼¶¼·¼¸¼¹¼º¼»¼¼¼½¼¾¼¿¼À¼Á¼Â¼Ã¼Ä¼Å¼Æ¼Ç¼È¼É¼Ê¼Ë¼Ì¼Í½åÆæÏµØ¢Ø½ØÀØŞÙ¥ÙÊÛÔÜ¸ÜÁÜùİ½İğŞªŞáß´ßÒßâßóá§áÕä©åæåìæ÷çÜçáé®éêêªê«êåêéê÷ì´í¶î¿ïúğ¢ñ¤ò±óÅóÇôßõÒõÕö«öİöê÷Ù÷ä"};
static const unsigned char Pymb_jia[]   ={"¼Î¼Ï¼Ğ¼Ñ¼Ò¼Ó¼Ô¼Õ¼Ö¼×¼Ø¼Ù¼Ú¼Û¼Ü¼İ¼ŞÇÑØÅÙ¤Û£İçáµä¤åÈçìê©ëÎí¢îòîşïØğèğıñÊòÌóÕôÂõÊ"};
static const unsigned char Pymb_jian[]  ={"¼ß¼à¼á¼â¼ã¼ä¼å¼æ¼ç¼è¼é¼ê¼ë¼ì¼í¼î¼ï¼ğ¼ñ¼ò¼ó¼ô¼õ¼ö¼÷¼ø¼ù¼ú¼û¼ü¼ı¼ş½¡½¢½£½¤½¥½¦½§½¨ÙÔÚÉÚÙÚÚİÑİóŞöàîäÕå¿åÀçÌèÅé¥ê§ê¯êğêùë¦ëìíúïµğÏñĞóÈôåõÂõİöä÷µ"};
static const unsigned char Pymb_jiang[] ={"ºç½©½ª½«½¬½­½®½¯½°½±½²½³½´½µÇ¿Üüä®ç­çÖêñíäñğôİôø"};
static const unsigned char Pymb_jiao[]  ={"½¶½·½¸½¹½º½»½¼½½½¾½¿½À½Á½Â½Ã½Ä½Å½Æ½Ç½È½É½Ê½Ë½Ì½Í½Î½Ï½Ğ½Ñ¾õĞ£Ù®ÙÕÜ´ÜúŞØàİá½áèæ¯ë¸ğ¨ğÔòÔõ´õÓöŞ"};
static const unsigned char Pymb_jie[]   ={"½Ò½Ó½Ô½Õ½Ö½×½Ø½Ù½Ú½Û½Ü½İ½Ş½ß½à½á½â½ã½ä½å½æ½ç½è½é½ê½ë½ìÙÊÚ¦ÚµÚàŞ×à®àµæ¼æİèîíÙïÇğÜò¡ò»ôÉöÚ÷º"};
static const unsigned char Pymb_jin[]   ={"½í½î½ï½ğ½ñ½ò½ó½ô½õ½ö½÷½ø½ù½ú½û½ü½ı½ş¾¡¾¢Úáİ£İÀàäâËâÛæ¡çÆèªéÈêáêîîÄñÆñæ"};
static const unsigned char Pymb_jing[]  ={"¾¢¾£¾¤¾¥¾¦¾§¾¨¾©¾ª¾«¾¬¾­¾®¾¯¾°¾±¾²¾³¾´¾µ¾¶¾·¾¸¾¹¾º¾»ØÙÙÓÚåÚêİ¼â°ã½ãşåÉåòæºëÂëÖëæìºö¦"};
static const unsigned char Pymb_jiong[] ={"¾¼¾½ØçåÄìç"};
static const unsigned char Pymb_jiu[]   ={"¾¾¾¿¾À¾Á¾Â¾Ã¾Ä¾Å¾Æ¾Ç¾È¾É¾Ê¾Ë¾Ì¾Í¾ÎÙÖà±ãÎèÑèêğ¯ğÕôñ÷İ"};
static const unsigned char Pymb_ju[]    ={"³µ¹ñ¾Ï¾Ğ¾Ñ¾Ò¾Ó¾Ô¾Õ¾Ö¾×¾Ø¾Ù¾Ú¾Û¾Ü¾İ¾Ş¾ß¾à¾á¾â¾ã¾ä¾å¾æ¾çÙÆÚªÜÄÜÚÜìŞäåáåğè¢èÛé§é°é·éÙêøì«îÒï¸ñÀñÕôòõ¶õáö´öÂöÄ÷¶"};
static const unsigned char Pymb_juan[]  ={"¾è¾é¾ê¾ë¾ì¾í¾îÈ¦Û²áúä¸èğîÃïÃïÔöÁ"};
static const unsigned char Pymb_jue[]   ={"½À½Å½Ç¾ï¾ğ¾ñ¾ò¾ó¾ô¾õ¾ö¾÷¾øØÊØãÚÜÛÇÜ¥Ş§àÙáÈâ±æŞçåèöéÓìßïãõêõû"};
static const unsigned char Pymb_jun[]   ={"¹ê¾ù¾ú¾û¾ü¾ı¾ş¿¡¿¢¿£¿¤¿¥ŞÜñäóŞ÷å"};

static const unsigned char Pymb_k[]     ={"¿´¿ª¿É¿ì¿Ë¿×"};
static const unsigned char Pymb_ka[]    ={"¿¦¿§¿¨¿©ØûßÇëÌ"};
static const unsigned char Pymb_kai[]   ={"¿ª¿«¿¬¿­¿®ØÜÛîİÜâéâıîøï´ïÇ"};
static const unsigned char Pymb_kan[]   ={"¼÷¿¯¿°¿±¿²¿³¿´Ù©ÛÉİ¨ãÛê¬íèî«"};
static const unsigned char Pymb_kang[]  ={"¿µ¿¶¿·¿¸¿¹¿º¿»ØøãÊîÖ"};
static const unsigned char Pymb_kao[]   ={"¿¼¿½¿¾¿¿åêèàêûîí"};
static const unsigned char Pymb_ke[]    ={"ºÇ¿À¿Á¿Â¿Ã¿Ä¿Å¿Æ¿Ç¿È¿É¿Ê¿Ë¿Ì¿Í¿Îà¾á³ã¡äÛæìç¼çæéğë´î§îİîşï¾ïığâñ½ò¤òÂòò÷Á"};
static const unsigned char Pymb_ken[]   ={"¿Ï¿Ğ¿Ñ¿ÒñÌ"};
static const unsigned char Pymb_keng[]  ={"¿Ó¿ÔëÖîïï¬"};
static const unsigned char Pymb_kong[]  ={"¿Õ¿Ö¿×¿ØÙÅáÇóí"};
static const unsigned char Pymb_kou[]   ={"¿Ù¿Ú¿Û¿ÜÜÒŞ¢ßµíîóØ"};
static const unsigned char Pymb_ku[]    ={"¿İ¿Ş¿ß¿à¿á¿â¿ãØÚÜ¥à·ç«÷¼"};
static const unsigned char Pymb_kua[]   ={"¿ä¿å¿æ¿ç¿èÙ¨ï¾"};
static const unsigned char Pymb_kuai[]  ={"»á¿é¿ê¿ë¿ìØáÛ¦ßàáöëÚ"};
static const unsigned char Pymb_kuan[]  ={"¿í¿î÷Å"};
static const unsigned char Pymb_kuang[] ={"¿ï¿ğ¿ñ¿ò¿ó¿ô¿õ¿öÚ²Ú¿Ú÷ÛÛŞÅßÑæşêÜ"};
static const unsigned char Pymb_kui[]   ={"¿÷¿ø¿ù¿ú¿û¿ü¿ı¿şÀ¡À¢À£Ø¸ØÑÙçÚóİŞŞñà­à°ã¦ã´åÓêÒî¥ñùòñóñõÍ"};
static const unsigned char Pymb_kun[]   ={"À¤À¥À¦À§ã§ãÍçûï¿õ«öï÷Õ"};
static const unsigned char Pymb_kuo[]   ={"À¨À©ÀªÀ«òÒ"};

static const unsigned char Pymb_l[]     ={"ÁËÁ½À´ÀïÀÏÂ¥"};
static const unsigned char Pymb_la[]    ={"À¬À­À®À¯À°À±À²ÂäØİååê¹íÇğø"};
static const unsigned char Pymb_lai[]   ={"À³À´ÀµáÁáâäµäşêãíùïªñ®ô¥"};
static const unsigned char Pymb_lan[]   ={"À¶À·À¸À¹ÀºÀ»À¼À½À¾À¿ÀÀÀÁÀÂÀÃÀÄá°äíé­ìµî½ïçñÜ"};
static const unsigned char Pymb_lang[]  ={"ÀÅÀÆÀÇÀÈÀÉÀÊÀËİ¹İõà¥ãÏï¶ïüòë"};
static const unsigned char Pymb_lao[]   ={"ÀÌÀÍÀÎÀÏÀĞÀÑÀÒÀÓÀÔÂäÂçßëáÀèáîîï©ğìñìõ²"};
static const unsigned char Pymb_le[]    ={"ÀÕÀÖÁËØìß·ãî÷¦"};
static const unsigned char Pymb_lei[]   ={"ÀÕÀ×ÀØÀÙÀÚÀÛÀÜÀİÀŞÀßÀàÀáÙúÚ³àÏæĞçĞéÛñçõª"};
static const unsigned char Pymb_leng[]  ={"ÀâÀãÀäÜ¨ã¶"};
static const unsigned char Pymb_li[]    ={"ÀåÀæÀçÀèÀéÀêÀëÀìÀíÀîÀïÀğÀñÀòÀóÀôÀõÀöÀ÷ÀøÀùÀúÀûÀüÀıÀşÁ¡Á¢Á£Á¤Á¥Á¦Á§Á¨ØªÙ³ÙµÛªÛŞÜÂİ°İñŞ¼ß¿à¦à¬áûäàå¢åÎæ²æËæêçÊèÀèİéöìåíÂíÇîºî¾ï®ğ¿ğİğßòÃòÛó»óÒóöôÏõ·õÈö¨öâ÷¯÷ó"};
static const unsigned char Pymb_lian[]  ={"Á©ÁªÁ«Á¬Á­Á®Á¯Á°Á±Á²Á³Á´ÁµÁ¶Á·İüŞÆäòå¥çöé¬éçì¡ñÍñÏó¹öã"};
static const unsigned char Pymb_liang[] ={"Á©Á¸Á¹ÁºÁ»Á¼Á½Á¾Á¿ÁÀÁÁÁÂÜ®İ¹é£õÔ÷Ë"};
static const unsigned char Pymb_liao[]  ={"ÁÃÁÄÁÅÁÆÁÇÁÈÁÉÁÊÁËÁÌÁÍÁÎÁÏŞ¤ŞÍàÚâ²å¼çÔîÉğÓ"};
static const unsigned char Pymb_lie[]   ={"ÁĞÁÑÁÒÁÓÁÔÙıÛøŞæßÖä£ôóõñ÷à"};
static const unsigned char Pymb_lin[]   ={"ÁÕÁÖÁ×ÁØÁÙÁÚÁÛÁÜÁİÁŞÁßÁàİşßøá×âŞãÁåàéİê¥ì¢î¬ôÔõï÷ë"};
static const unsigned char Pymb_ling[]  ={"ÀâÁáÁâÁãÁäÁåÁæÁçÁèÁéÁêÁëÁìÁíÁîÛ¹ÜßßÊàòãöç±èÚèùê²ñöòÈôáöì"};
static const unsigned char Pymb_liu[]   ={"ÁïÁğÁñÁòÁóÁôÁõÁöÁ÷ÁøÁùÂµÂ½ä¯åŞæòç¸ì¼ìÖï³ïÖğÒöÌ"};
static const unsigned char Pymb_lo[]    ={"¿©"};
static const unsigned char Pymb_long[]  ={"ÁúÁûÁüÁıÁşÂ¡Â¢Â£Â¤ÅªÛâÜ×ãñççèĞëÊíÃñª"};
static const unsigned char Pymb_lou[]   ={"Â¥Â¦Â§Â¨Â©ÂªÂ¶ÙÍİäà¶áĞïÎğüñïò÷÷Ã"};
static const unsigned char Pymb_lu[]    ={"ÁùÂ«Â¬Â­Â®Â¯Â°Â±Â²Â³Â´ÂµÂ¶Â·Â¸Â¹ÂºÂ»Â¼Â½Â¾ÂÌÛäß£ààãòäËäõåÖè´èÓéÖéñéûê¤êÚëªëÍïåğµğØóüôµöÔ"};
static const unsigned char Pymb_lv[]    ={"Â¿ÂÀÂÁÂÂÂÃÂÄÂÅÂÆÂÇÂÈÂÉÂÊÂËÂÌÙÍŞÛãÌéµëöïùñÚ"};
static const unsigned char Pymb_lue[]   ={"ÂÓÂÔï²"};
static const unsigned char Pymb_luan[]  ={"ÂÍÂÎÂÏÂĞÂÑÂÒÙõæ®èïğ½öÇ"};
static const unsigned char Pymb_lun[]   ={"ÂÕÂÖÂ×ÂØÂÙÂÚÂÛàğ"};
static const unsigned char Pymb_luo[]   ={"¸õ¿©ÀÓÂÜÂİÂŞÂßÂàÂáÂâÂãÂäÂåÂæÂçÙÀÙùÜıŞÛŞûâ¤ãøäğçóé¡ëáíÑïİñ§öÃ"};

static const unsigned char Pymb_m[]     ={"ß¼ÂğÃÇÃ»ÂíÃÅÃæ"};
static const unsigned char Pymb_ma[]    ={"ÂèÂéÂêÂëÂìÂíÂîÂïÂğÄ¦Ä¨ßéáïæÖè¿ó¡"};
static const unsigned char Pymb_mai[]   ={"ÂñÂòÂóÂôÂõÂöÛ½İ¤ö²"};
static const unsigned char Pymb_man[]   ={"ÂñÂ÷ÂøÂùÂúÂûÂüÂıÂşÃ¡Ü¬á£çÏì×ïÜò©òı÷©÷´"};
static const unsigned char Pymb_mang[]  ={"Ã¢Ã£Ã¤Ã¥Ã¦Ã§ÚøäİíËòş"};
static const unsigned char Pymb_mao[]   ={"Ã¨Ã©ÃªÃ«Ã¬Ã­Ã®Ã¯Ã°Ã±Ã²Ã³ÙóÜâá¹ã÷è£êÄêóë£ì¸í®î¦òúó±÷Ö"};
static const unsigned char Pymb_me[]    ={"Ã´"};
static const unsigned char Pymb_mei[]   ={"ÃµÃ¶Ã·Ã¸Ã¹ÃºÃ»Ã¼Ã½Ã¾Ã¿ÃÀÃÁÃÂÃÃÃÄİ®áÒâ­ä¼äØé¹ïÑğÌñÇ÷È"};
static const unsigned char Pymb_men[]   ={"ÃÅÃÆÃÇŞÑìËí¯îÍ"};
static const unsigned char Pymb_meng[]  ={"ÃÈÃÉÃÊÃËÃÌÃÍÃÎÃÏÛÂİùŞ«ãÂëüíæòµòìó·ô»ô¿"};
static const unsigned char Pymb_mi[]    ={"ÃĞÃÑÃÒÃÓÃÔÃÕÃÖÃ×ÃØÃÙÃÚÃÛÃÜÃİØÂÚ¢Ú×ŞÂßäà×â¨ãèåµåôæùëßìòôÍôé÷ã÷ç"};
static const unsigned char Pymb_mian[]  ={"ÃŞÃßÃàÃáÃâÃãÃäÃåÃæãæäÅäÏå²ëïíí"};
static const unsigned char Pymb_miao[]  ={"ÃçÃèÃéÃêÃëÃìÃíÃîß÷åãç¿çÑèÂíµíğğÅ"};
static const unsigned char Pymb_mie[]   ={"ÃïÃğØ¿ßãóºóú"};
static const unsigned char Pymb_min[]   ={"ÃñÃòÃóÃôÃõÃöÜåáºãÉãıçÅçäçëíªö¼÷ª"};
static const unsigned char Pymb_ming[]  ={"Ã÷ÃøÃùÃúÃûÃüÚ¤ÜøäéêÔî¨õ¤"};
static const unsigned char Pymb_miu[]   ={"ÃıçÑ"};
static const unsigned char Pymb_mo[]    ={"ÂöÃ»ÃşÄ¡Ä¢Ä£Ä¤Ä¥Ä¦Ä§Ä¨Ä©ÄªÄ«Ä¬Ä­Ä®Ä¯Ä°ÍòÚÓÜÔİëâÉæÆéâïÒï÷ñ¢ñòõöõø÷á"};
static const unsigned char Pymb_mou[]   ={"Ä±Ä²Ä³Ù°ßèçÑíøòÖöÊ"};
static const unsigned char Pymb_mu[]    ={"Ä£Ä²Ä´ÄµÄ¶Ä·Ä¸Ä¹ÄºÄ»Ä¼Ä½Ä¾Ä¿ÄÀÄÁÄÂØïÛéÜÙãåë¤îâ"};

static const unsigned char Pymb_n[]     ={"ÄãÄÇÄêÄÚÄÜÄĞ"};
static const unsigned char Pymb_na[]    ={"ÄÃÄÄÄÅÄÆÄÇÄÈÄÉŞàëÇïÕñÄ"};
static const unsigned char Pymb_nai[]   ={"ÄÊÄËÄÌÄÍÄÎØ¾Ù¦ÜµİÁèÍ"};
static const unsigned char Pymb_nan[]   ={"ÄÏÄĞÄÑà«àïéªëîòïôö"};
static const unsigned char Pymb_nang[]  ={"ÄÒß­àìâÎêÙ"};
static const unsigned char Pymb_nao[]   ={"ÄÓÄÔÄÕÄÖÄ×Ø«ÛñßÎâ®è§íĞîóòÍ"};
static const unsigned char Pymb_ne[]    ={"ÄÄÄØÚ«"};
static const unsigned char Pymb_nei[]   ={"ÄÙÄÚ"};
static const unsigned char Pymb_nen[]   ={"ÄÛí¥"};
static const unsigned char Pymb_neng[]  ={"ÄÜ"};
static const unsigned char Pymb_ni[]    ={"ÄØÄİÄŞÄßÄàÄáÄâÄãÄäÄåÄæÄçÙ£Ûèâ¥âõêÇì»ìòí«íşîêöò"};
static const unsigned char Pymb_nian[]  ={"ÄèÄéÄêÄëÄìÄíÄîØ¥Ûşéığ¤öÓöó"};
static const unsigned char Pymb_niang[] ={"ÄïÄğ"};
static const unsigned char Pymb_niao[]  ={"ÄñÄòÜàæÕëåôÁ"};
static const unsigned char Pymb_nie[]   ={"ÄóÄôÄõÄöÄ÷ÄøÄùØ¿ÚíŞÁà¿ò¨ô«õæ"};
static const unsigned char Pymb_nin[]   ={"Äú"};
static const unsigned char Pymb_ning[]  ={"ÄûÄüÄıÄşÅ¡Å¢ØúßÌå¸ñ÷"};
static const unsigned char Pymb_niu[]   ={"Å£Å¤Å¥Å¦ŞÖáğâîæ¤"};
static const unsigned char Pymb_nong[]  ={"Å§Å¨Å©ÅªÙ¯ßæ"};
static const unsigned char Pymb_nou[]   ={"ññ"};
static const unsigned char Pymb_nu[]    ={"Å«Å¬Å­åóæÀæÛæå"};
static const unsigned char Pymb_nv[]    ={"Å®í¤îÏô¬"};
static const unsigned char Pymb_nue[]   ={"Å°Å±"};
static const unsigned char Pymb_nuan[]  ={"Å¯"};
static const unsigned char Pymb_nuo[]   ={"ÄÈÅ²Å³Å´ÅµÙĞŞùßöï»"};

static const unsigned char Pymb_o[]     ={"Å¶à¸àŞ"};
static const unsigned char Pymb_ou[]    ={"Å·Å¸Å¹ÅºÅ»Å¼Å½ÇøÚ©âæê±ñî"};

static const unsigned char Pymb_p[]     ={"Æ½Æ¬Æ·ÆÀÆ±ÅÂ"};
static const unsigned char Pymb_pa[]    ={"°Ç°ÒÅ¾Å¿ÅÀÅÁÅÂÅÃİâèËóá"};
static const unsigned char Pymb_pai[]   ={"ÅÄÅÅÅÆÅÇÅÈÅÉÆÈÙ½İåßß"};
static const unsigned char Pymb_pan[]   ={"·¬ÅÊÅËÅÌÅÍÅÎÅÏÅĞÅÑÅÖŞÕãÜãİãúêÚñÈñáó´õç"};
static const unsigned char Pymb_pang[]  ={"°ò°õ°÷ÅÒÅÓÅÔÅÕÅÖáİäèåÌó¦"};
static const unsigned char Pymb_pao[]   ={"Å×ÅØÅÙÅÚÅÛÅÜÅİŞËáóâÒëãğå"};
static const unsigned char Pymb_pei[]   ={"ÅŞÅßÅàÅáÅâÅãÅäÅåÅæàÎàúì·ïÂõ¬ö¬"};
static const unsigned char Pymb_pen[]   ={"ÅçÅèäÔ"};
static const unsigned char Pymb_peng[]  ={"ÅéÅêÅëÅìÅíÅîÅïÅğÅñÅòÅóÅôÅõÅöÜ¡àØâñó²"};
static const unsigned char Pymb_pi[]    ={"±Ù·ñÅ÷ÅøÅùÅúÅûÅüÅıÅşÆ¡Æ¢Æ£Æ¤Æ¥Æ¦Æ§Æ¨Æ©Ø§ØòÚéÚğÚüÛ¯ÛÜÛıÜ±ÜÅß¨àèâÏäÄæÇç¢èÁê¶î¢î¼îëñ±ñÔñâò·òçõù"};
static const unsigned char Pymb_pian[]  ={"±â±ãÆªÆ«Æ¬Æ­ÚÒæéçÂêúëİôæõä"};
static const unsigned char Pymb_piao[]  ={"Æ®Æ¯Æ°Æ±ÆÓØâàÑæÎæôçÎéèî©óª"};
static const unsigned char Pymb_pie[]   ={"Æ²Æ³Ø¯ÜÖë­"};
static const unsigned char Pymb_pin[]   ={"Æ´ÆµÆ¶Æ·Æ¸æ°æÉé¯êòò­"};
static const unsigned char Pymb_ping[]  ={"·ëÆ¹ÆºÆ»Æ¼Æ½Æ¾Æ¿ÆÀÆÁÙ·æ³èÒöÒ"};
static const unsigned char Pymb_po[]    ={"²´·±ÆÂÆÃÆÄÆÅÆÆÆÇÆÈÆÉÆÓØÏÚéÛ¶çêê·ë¶îÇîŞğ«óÍ"};
static const unsigned char Pymb_pou[]   ={"ÆÊÙöŞå"};
static const unsigned char Pymb_pu[]    ={"±¤±©¸¬ÆËÆÌÆÍÆÎÆÏÆĞÆÑÆÒÆÓÆÔÆÕÆÖÆ×ÆØÆÙÙéàÛäßå§è±ë«ïäïèõë"};

static const unsigned char Pymb_q[]     ={"ÆßÈ¥ÆäÈ«ÆÚÇ°"};
static const unsigned char Pymb_qi[]    ={"»ü¼©ÆÚÆÛÆÜÆİÆŞÆßÆàÆáÆâÆãÆäÆåÆæÆçÆèÆéÆêÆëÆìÆíÆîÆïÆğÆñÆòÆóÆôÆõÆöÆ÷ÆøÆùÆúÆûÆüÆıØ½ØÁÙ¹ÛßÜ»ÜÎÜùİÂİİŞ­àÒá¨áªãàä¿æëç²ç÷çùè½èçéÊêÈì¥ì÷í¬íÓñıòÓòàôëôìõè÷¢÷è"};
static const unsigned char Pymb_qia[]   ={"¿¨ÆşÇ¡Ç¢İÖñÊ÷Ä"};
static const unsigned char Pymb_qian[]  ={"Ç£Ç¤Ç¥Ç¦Ç§Ç¨Ç©ÇªÇ«Ç¬Ç­Ç®Ç¯Ç°Ç±Ç²Ç³Ç´ÇµÇ¶Ç·Ç¸ÏËÙ»ÙİÚäÜ·ÜÍÜçİ¡Şçá©ã¥ã»å¹åºå½ç×èıêùëÉí©îÔò¯óéôÇ"};
static const unsigned char Pymb_qiang[] ={"Ç¹ÇºÇ»Ç¼Ç½Ç¾Ç¿ÇÀãÜãŞæÍéÉê¨ìÁïÏïêñßòŞôÇõÄ"};
static const unsigned char Pymb_qiao[]  ={"¿ÇÇÁÇÂÇÃÇÄÇÅÇÆÇÇÇÈÇÉÇÊÇËÇÌÇÍÇÎÇÏÈ¸ØäÚ½ÚÛÜñá½ã¸ã¾çØéÔíÍï¢õÎ÷³"};
static const unsigned char Pymb_qie[]   ={"ÇĞÇÑÇÒÇÓÇÔÙ¤Û§ã«ã»æªêüïÆóæôò"};
static const unsigned char Pymb_qin[]   ={"ÇÕÇÖÇ×ÇØÇÙÇÚÇÛÇÜÇİÇŞÇßÜËŞìßÄàºàßâÛéÕï·ñæñûòûôÀ"};
static const unsigned char Pymb_qing[]  ={"Ç×ÇàÇáÇâÇãÇäÇåÇæÇçÇèÇéÇêÇëÇìÜÜàõéÑíàïºòßóÀóäôìö¥öë÷ô"};
static const unsigned char Pymb_qiong[] ={"ÇíÇîÚöÜäñ·òËóÌõ¼öÆ"};
static const unsigned char Pymb_qiu[]   ={"³ğ¹êÇïÇğÇñÇòÇóÇôÇõÇöÙ´ÛÏáìäĞåÏåÙé±êäò°òÇòøôÃôÜöú÷ü"};
static const unsigned char Pymb_qu[]    ={"Ç÷ÇøÇùÇúÇûÇüÇıÇşÈ¡È¢È£È¤È¥ĞçÚ°Û¾ÛÉÜÄŞ¡Ş¾á«áéãÖè³êïë¬ëÔìîíáğ¶ñ³òĞó½ôğöÄ÷ñ"};
static const unsigned char Pymb_quan[]  ={"È¦È§È¨È©ÈªÈ«È¬È­È®È¯È°Ú¹Üõáëãªç¹éúî°îıòéóÜ÷Ü"};
static const unsigned char Pymb_que[]   ={"È±È²È³È´ÈµÈ¶È·È¸ã×ãÚí¨"};
static const unsigned char Pymb_qun[]   ={"È¹ÈºåÒ÷å"};

static const unsigned char Pymb_r[]     ={"ÈËÈçÈÕÈÃÈôÈë"};
static const unsigned char Pymb_ran[]   ={"È»È¼È½È¾ÜÛòÅ÷×"};
static const unsigned char Pymb_rang[]  ={"È¿ÈÀÈÁÈÂÈÃìüğ¦"};
static const unsigned char Pymb_rao[]   ={"ÈÄÈÅÈÆÜéæ¬èã"};
static const unsigned char Pymb_re[]    ={"ÈÇÈÈßö"};
static const unsigned char Pymb_ren[]   ={"ÈÉÈÊÈËÈÌÈÍÈÎÈÏÈĞÈÑÈÒØéØğÜóİØâ¿éíïşñÅ"};
static const unsigned char Pymb_reng[]  ={"ÈÓÈÔ"};
static const unsigned char Pymb_ri[]    ={"ÈÕ"};
static const unsigned char Pymb_rong[]  ={"ÈÖÈ×ÈØÈÙÈÚÈÛÈÜÈİÈŞÈßáÉáõéÅëÀòî"};
static const unsigned char Pymb_rou[]   ={"ÈàÈáÈâôÛõå÷·"};
static const unsigned char Pymb_ru[]    ={"ÈãÈäÈåÈæÈçÈèÈéÈêÈëÈìİêŞ¸àéä²äáå¦çÈï¨ñàò¬"};
static const unsigned char Pymb_ruan[]  ={"ÈíÈîëÃ"};
static const unsigned char Pymb_rui[]   ={"ÈïÈğÈñÜÇŞ¨èÄî£ò¸"};
static const unsigned char Pymb_run[]   ={"ÈòÈó"};
static const unsigned char Pymb_ruo[]   ={"ÈôÈõÙ¼óè"};

static const unsigned char Pymb_s[]     ={"ÊÇËÄÉÏËµÊÂÊ±"};
static const unsigned char Pymb_sa[]    ={"ÈöÈ÷ÈøØ¦ØíëÛìª"};
static const unsigned char Pymb_sai[]   ={"ÈùÈúÈûÈüàç"};
static const unsigned char Pymb_san[]   ={"ÈıÈşÉ¡É¢âÌë§ôÖ"};
static const unsigned char Pymb_sang[]  ={"É£É¤É¥Şúíßòª"};
static const unsigned char Pymb_sao[]   ={"É¦É§É¨É©Ü£çÒëığşöş"};
static const unsigned char Pymb_se[]    ={"ÈûÉªÉ«É¬ØÄï¤ğ£"};
static const unsigned char Pymb_sen[]   ={"É­"};
static const unsigned char Pymb_seng[]  ={"É®"};
static const unsigned char Pymb_sha[]   ={"É¯É°É±É²É³É´ÉµÉ¶É·É¼ÏÃßşì¦ï¡ğğôÄö®öè"};
static const unsigned char Pymb_shai[]  ={"É«É¸É¹"};
static const unsigned char Pymb_shan[]  ={"²ôµ¥ÉºÉ»É¼É½É¾É¿ÉÀÉÁÉÂÉÃÉÄÉÅÉÆÉÇÉÈÉÉÕ¤ØßÚ¨Û·ÛïÜÏáêäúæ©æÓæóëşìøîÌğŞóµô®õÇ÷­÷Ô"};
static const unsigned char Pymb_shang[] ={"ÉÊÉËÉÌÉÍÉÎÉÏÉĞÉÑÛğç´éäìØõü"};
static const unsigned char Pymb_shao[]  ={"ÇÊÉÒÉÓÉÔÉÕÉÖÉ×ÉØÉÙÉÚÉÛÉÜÛ¿Üæäûè¼òÙóâô¹"};
static const unsigned char Pymb_she[]   ={"ÉİÉŞÉßÉàÉáÉâÉãÉäÉåÉæÉçÉèÕÛØÇÙÜŞéâ¦äÜì¨î´îè÷ê"};
static const unsigned char Pymb_shen[]  ={"²ÎÉéÉêÉëÉìÉíÉîÉïÉğÉñÉòÉóÉôÉõÉöÉ÷ÉøÊ²Ú·ÚÅİ·İØßÓäÉé©ëÏïòò×"};
static const unsigned char Pymb_sheng[] ={"³ËÉùÉúÉûÉüÉıÉşÊ¡Ê¢Ê£Ê¤Ê¥áÓäÅêÉíòóÏ"};
static const unsigned char Pymb_shi[]   ={"³×Ê¦Ê§Ê¨Ê©ÊªÊ«Ê¬Ê­Ê®Ê¯Ê°Ê±Ê²Ê³Ê´ÊµÊ¶Ê·Ê¸Ê¹ÊºÊ»Ê¼Ê½Ê¾Ê¿ÊÀÊÁÊÂÊÃÊÄÊÅÊÆÊÇÊÈÊÉÊÊÊËÊÌÊÍÊÎÊÏÊĞÊÑÊÒÊÓÊÔËÆÖ³ÖÅÚÖÛõİªİéß±â»éøêÛìÂìêîæîèó§óÂóßõ§õ¹öåöõ"};
static const unsigned char Pymb_shou[]  ={"ÊÕÊÖÊ×ÊØÊÙÊÚÊÛÊÜÊİÊŞŞĞá÷ç·ô¼"};
static const unsigned char Pymb_shu[]   ={"ÊßÊàÊáÊâÊãÊäÊåÊæÊçÊèÊéÊêÊëÊìÊíÊîÊïÊğÊñÊòÊóÊôÊõÊöÊ÷ÊøÊùÊúÊûÊüÊıÊşË¡Ø­Ù¿ÛÓİÄŞóãğäøæ­ç£ë¨ëòì¯ïíïøñâ"};
static const unsigned char Pymb_shua[]  ={"Ë¢Ë£à§"};
static const unsigned char Pymb_shuai[] ={"ÂÊË¤Ë¥Ë¦Ë§ó°"};
static const unsigned char Pymb_shuan[] ={"Ë¨Ë©ãÅäÌ"};
static const unsigned char Pymb_shuang[]={"ËªË«Ë¬ãñæ×"};
static const unsigned char Pymb_shui[]  ={"Ë­Ë®Ë¯Ë°Ëµãß"};
static const unsigned char Pymb_shun[]  ={"Ë±Ë²Ë³Ë´"};
static const unsigned char Pymb_shuo[]  ={"ÊıËµË¶Ë·Ë¸İôŞ÷åùéÃîå"};
static const unsigned char Pymb_si[]    ={"Ë¹ËºË»Ë¼Ë½Ë¾Ë¿ËÀËÁËÂËÃËÄËÅËÆËÇËÈØËÙ¹ÙîÛÌßĞâ»ãáãôäùæ¦æáçÁìëïÈğ¸ñêòÏóÓ"};
static const unsigned char Pymb_song[]  ={"ËÉËÊËËËÌËÍËÎËÏËĞÚ¡İ¿áÂáÔâìã¤äÁñµ"};
static const unsigned char Pymb_sou[]   ={"ËÑËÒËÓËÔÛÅŞ´à²àÕâÈäÑì¬î¤ïËòô"};
static const unsigned char Pymb_su[]    ={"ËÕËÖË×ËØËÙËÚËÛËÜËİËŞËßËàËõÙíÚÕİøà¼ãºä³óùö¢öÕ"};
static const unsigned char Pymb_suan[]  ={"ËáËâËãâ¡"};
static const unsigned char Pymb_sui[]   ={"ÄòËäËåËæËçËèËéËêËëËìËíËîÚÇİ´å¡åäìİíõî¡"};
static const unsigned char Pymb_sun[]   ={"ËïËğËñİ¥áøâ¸é¾öÀ"};
static const unsigned char Pymb_suo[]   ={"É¯ËòËóËôËõËöË÷ËøËùßïàÂàÊæ¶èøêıíüôÈ"};

static const unsigned char Pymb_t[]     ={"ËûËıÌ«Í·ÌìÍ¬"};
static const unsigned char Pymb_ta[]    ={"ËúËûËüËıËşÌ¡Ì¢Ì£Ì¤ÍØãËäâäğåİé½í³îèõÁ÷£"};
static const unsigned char Pymb_tai[]   ={"Ì¥Ì¦Ì§Ì¨Ì©ÌªÌ«Ì¬Ì­Û¢Ş·ææëÄìÆîÑõÌöØ"};
static const unsigned char Pymb_tan[]   ={"µ¯Ì®Ì¯Ì°Ì±Ì²Ì³Ì´ÌµÌ¶Ì·Ì¸Ì¹ÌºÌ»Ì¼Ì½Ì¾Ì¿Û°å£ê¼êæìşîãïÄïâñû"};
static const unsigned char Pymb_tang[]  ={"ÌÀÌÁÌÂÌÃÌÄÌÅÌÆÌÇÌÈÌÉÌÊÌËÌÌÙÎàûâ¼äçè©éÌîõï¦ïÛñíó¥ó«ôÊõ±"};
static const unsigned char Pymb_tao[]   ={"ÌÍÌÎÌÏÌĞÌÑÌÒÌÓÌÔÌÕÌÖÌ×Ø»ß¶ßûä¬èºìâ÷Ò"};
static const unsigned char Pymb_te[]    ={"ÌØß¯ìıï«"};
static const unsigned char Pymb_teng[]  ={"ÌÙÌÚÌÛÌÜëø"};
static const unsigned char Pymb_ti[]    ={"ÌİÌŞÌßÌàÌáÌâÌãÌäÌåÌæÌçÌèÌéÌêÌëÙÃÜèã©åÑç°ç¾ğÃñÓõ®"};
static const unsigned char Pymb_tian[]  ={"ÌìÌíÌîÌïÌğÌñÌòÌóŞİãÃãÙéåî±îäï»"};
static const unsigned char Pymb_tiao[]  ={"µ÷ÌôÌõÌöÌ÷ÌøÙ¬Üæìöï¢ñ»òèóÔôĞö¶öæ÷Ø"};
static const unsigned char Pymb_tie[]   ={"ÌùÌúÌûİÆï°÷Ñ"};
static const unsigned char Pymb_ting[]  ={"ÌüÌıÌşÍ¡Í¢Í£Í¤Í¥Í¦Í§ÜğİãæÃèèîúòÑöª"};
static const unsigned char Pymb_tong[]  ={"Í¨Í©ÍªÍ«Í¬Í­Í®Í¯Í°Í±Í²Í³Í´Ù¡Ù×ÙÚÛíÜíàÌá¼âúäüíÅ"};
static const unsigned char Pymb_tou[]   ={"ÍµÍ¶Í·Í¸Ùïî×÷»"};
static const unsigned char Pymb_tu[]    ={"Í¹ÍºÍ»Í¼Í½Í¾Í¿ÍÀÍÁÍÂÍÃÜ¢İ±İËîÊõ©"};
static const unsigned char Pymb_tuan[]  ={"ÍÄÍÅŞÒåèî¶"};
static const unsigned char Pymb_tui[]   ={"ÍÆÍÇÍÈÍÉÍÊÍËìÕ"};
static const unsigned char Pymb_tun[]   ={"¶ÚÍÊÍÌÍÍÍÎÙÛâ½êÕëà"};
static const unsigned char Pymb_tuo[]   ={"ËµÍÏÍĞÍÑÍÒÍÓÍÔÍÕÍÖÍ×ÍØÍÙØ±Ù¢ÛçâÕãûèØèŞéÒíÈîèóêõ¢õÉö¾"};

static const unsigned char Pymb_w[]     ={"ÎÒÎªÎåÍòÎŞÍõ"};
static const unsigned char Pymb_wa[]    ={"ÍÚÍÛÍÜÍİÍŞÍßÍàØôæ´ëğ"};
static const unsigned char Pymb_wai[]   ={"ÍáÍâáË"};
static const unsigned char Pymb_wan[]   ={"ÂûÍãÍäÍåÍæÍçÍèÍéÍêÍëÍìÍíÍîÍïÍğÍñÍòÍóØàÜ¹İ¸İÒæıçºçşëäîµòê÷´"};
static const unsigned char Pymb_wang[]  ={"ÍôÍõÍöÍ÷ÍøÍùÍúÍûÍüÍıØèŞÌã¯éş÷Í"};
static const unsigned char Pymb_wei[]   ={"ÍşÎ¡Î¢Î£Î¤Î¥Î¦Î§Î¨Î©ÎªÎ«Î¬Î­Î®Î¯Î°Î±Î²Î³Î´ÎµÎ¶Î·Î¸Î¹ÎºÎ»Î¼Î½Î¾Î¿ÎÀÙËÚÃÚñÚóÛ×İÚŞ±àíàøá¡áËáÍâ«â¬ãÇãíä¢ä¶åÔæ¸çâè¸ê¦ì¿ìĞğôôºöÛ"};
static const unsigned char Pymb_wen[]   ={"ÎÁÎÂÎÃÎÄÎÅÎÆÎÇÎÈÎÉÎÊØØãÓãëçäè·ö©"};
static const unsigned char Pymb_weng[]  ={"ÎËÎÌÎÍİîŞ³"};
static const unsigned char Pymb_wo[]    ={"ÎÎÎÏÎĞÎÑÎÒÎÓÎÔÎÕÎÖÙÁİ«à¸á¢ä×ë¿íÒö»"};
static const unsigned char Pymb_wu[]    ={"¶ñÎ×ÎØÎÙÎÚÎÛÎÜÎİÎŞÎßÎàÎáÎâÎãÎäÎåÎæÎçÎèÎéÎêÎëÎìÎíÎîÎïÎğÎñÎòÎóØ£ØõÚãÚùÛØÜÌßíâĞâäâèä´å»åÃåüæÄæğè»êõì¶ìÉğÄğÍğíòÚöÈ÷ù"};

static const unsigned char Pymb_x[]     ={"ÏëĞ¡ÏÂÏÈÑ§ÏòÏÖĞÄÏñĞÍĞÂÏØ"};
static const unsigned char Pymb_xi[]    ={"ÎôÎõÎöÎ÷ÎøÎùÎúÎûÎüÎıÎşÏ¡Ï¢Ï£Ï¤Ï¥Ï¦Ï§Ï¨Ï©ÏªÏ«Ï¬Ï­Ï®Ï¯Ï°Ï±Ï²Ï³Ï´ÏµÏ¶Ï·Ï¸ØÎÙÒÙâÚôÛ­Üçİ¾İßİûŞÉßñáãâ¾ãÒä»äÀåïæÒçôéØêØêêì¤ì¨ìäìùìûğªñ¶ñÓòáó£ó¬ôªô¸ôËôÑôâõµõè÷û"};
static const unsigned char Pymb_xia[]   ={"Ï¹ÏºÏ»Ï¼Ï½Ï¾Ï¿ÏÀÏÁÏÂÏÃÏÄÏÅßÈáòåÚè¦èÔíÌóÁ÷ï"};
static const unsigned char Pymb_xian[]  ={"Ï³Ï´ÏÆÏÇÏÈÏÉÏÊÏËÏÌÏÍÏÎÏÏÏĞÏÑÏÒÏÓÏÔÏÕÏÖÏ×ÏØÏÙÏÚÏÛÏÜÏİÏŞÏßÙşÜÈİ²Şºá­áıåßæµë¯ìŞììğÂğçğïò¹óÚôÌõ£õĞõÑö±"};
static const unsigned char Pymb_xiang[] ={"½µÏàÏáÏâÏãÏäÏåÏæÏçÏèÏéÏêÏëÏìÏíÏîÏïÏğÏñÏòÏóÜ¼İÙâÃâÔæøç½ó­öß÷Ï"};
static const unsigned char Pymb_xiao[]  ={"ÏôÏõÏöÏ÷ÏøÏùÏúÏûÏüÏıÏşĞ¡Ğ¢Ğ£Ğ¤Ğ¥Ğ¦Ğ§ßØäìåĞæçç¯èÉèÕòÙóãóï÷Ì"};
static const unsigned char Pymb_xie[]   ={"½âĞ¨Ğ©ĞªĞ«Ğ¬Ğ­Ğ®Ğ¯Ğ°Ğ±Ğ²Ğ³Ğ´ĞµĞ¶Ğ·Ğ¸Ğ¹ĞºĞ»Ğ¼ÑªÒ¶ÙÉÙôÛÄÛÆŞ¯ß¢â³âİäÍå¬åâç¥çÓé¿éÇò¡õóöÙ÷º"};
static const unsigned char Pymb_xin[]   ={"Ğ½Ğ¾Ğ¿ĞÀĞÁĞÂĞÃĞÄĞÅĞÆØ¶Ü°İ·âàê¿ì§ïâöÎ"};
static const unsigned char Pymb_xing[]  ={"Ê¡ĞÇĞÈĞÉĞÊĞËĞÌĞÍĞÎĞÏĞĞĞÑĞÒĞÓĞÔĞÕÚêÜôÜşß©â¼ã¬íÊ"};
static const unsigned char Pymb_xiong[] ={"ĞÖĞ×ĞØĞÙĞÚĞÛĞÜÜº"};
static const unsigned char Pymb_xiu[]   ={"³ôËŞĞİĞŞĞßĞàĞáĞâĞãĞäĞåßİá¶âÊâÓäåğ¼õ÷÷Û"};
static const unsigned char Pymb_xu[]    ={"ĞæĞçĞèĞéĞêĞëĞìĞíĞîĞïĞğĞñĞòĞóĞôĞõĞöĞ÷ĞøÓõÚ¼ÛÃÛ×Ş£äªä°äÓçïèòìãíìñãôÚõ¯"};
static const unsigned char Pymb_xuan[]  ={"È¯ĞùĞúĞûĞüĞıĞşÑ¡Ñ¢Ñ£Ñ¤ÙØÚÎİæŞïãùäÖäöè¯é¸êÑìÅìÓíÛîçïà"};
static const unsigned char Pymb_xue[]   ={"Ï÷Ñ¥Ñ¦Ñ§Ñ¨Ñ©ÑªÚÊàåí´õ½÷¨"};
static const unsigned char Pymb_xun[]   ={"¿£Ñ«Ñ¬Ñ­Ñ®Ñ¯Ñ°Ñ±Ñ²Ñ³Ñ´ÑµÑ¶Ñ·Ñ¸ÙãÛ¨Û÷Ü÷İ¡Ş¦Ş¹á¾áßâ´âşä­ä±êÖñ¿õ¸öà"};

static const unsigned char Pymb_y[]     ={"Ò»ÔÂÒ²ÓĞÓÃÒª"};
static const unsigned char Pymb_ya[]    ={"Ñ¹ÑºÑ»Ñ¼Ñ½Ñ¾Ñ¿ÑÀÑÁÑÂÑÃÑÄÑÅÑÆÑÇÑÈÔşØóÛëŞëá¬åÂæ«çğèâë²í¼íığéñâ"};
static const unsigned char Pymb_yan[]   ={"Ç¦ÑÉÑÊÑËÑÌÑÍÑÎÑÏÑĞÑÑÑÒÑÓÑÔÑÕÑÖÑ×ÑØÑÙÑÚÑÛÑÜÑİÑŞÑßÑàÑáÑâÑãÑäÑåÑæÑçÑèÑéÒóØÉØÍØßÙ²ÙÈÙğÚ¥ÚİÚçÛ±Û³Ü¾İÎáÃâûãÆãÕäÎäÙåûæÌçüéÜêÌëÙëçìÍî»óÛõ¦÷Ê÷Ğ÷ú÷ü"};
static const unsigned char Pymb_yang[]  ={"ÑêÑëÑìÑíÑîÑïÑğÑñÑòÑóÑôÑõÑöÑ÷ÑøÑùÑúáàâóãóì¾ìÈí¦òÕ÷±"};
static const unsigned char Pymb_yao[]   ={"½ÄÄöÅ±ÑûÑüÑıÑşÒ¡Ò¢Ò£Ò¤Ò¥Ò¦Ò§Ò¨Ò©ÒªÒ«Ô¿Ø²Ø³ßºáÅáÊáæçÛçòèÃé÷ê×ëÈï¢ğÎñºôí÷¥"};
static const unsigned char Pymb_ye[]    ={"Ğ°ÑÊÒ¬Ò­Ò®Ò¯Ò°Ò±Ò²Ò³Ò´ÒµÒ¶Ò·Ò¸Ò¹ÒºÖÑØÌÚËÚşŞŞçğêÊìÇîô"};
static const unsigned char Pymb_yi[]    ={"°¬Î²Ò»Ò¼Ò½Ò¾Ò¿ÒÀÒÁÒÂÒÃÒÄÒÅÒÆÒÇÒÈÒÉÒÊÒËÒÌÒÍÒÎÒÏÒĞÒÑÒÒÒÓÒÔÒÕÒÖÒ×ÒØÒÙÒÚÒÛÒÜÒİÒŞÒßÒàÒáÒâÒãÒäÒåÒæÒçÒèÒéÒêÒëÒìÒíÒîÒïØ×ØæØıÙ«Ú±ÚâÛİÛüÜ²ÜÓÜèŞ²ŞÄŞÈŞÚß®ß½ß×ßŞàæá»áÚâ¢âÂâøâùã¨äôåÆæäçËéìéóêİì¥ì½ìÚíôîÆîèï×ïîğêğùñ¯ñ´ñÂòæô¯ôàôèôı÷ğ"};
static const unsigned char Pymb_yin[]   ={"ÒğÒñÒòÒóÒôÒõÒöÒ÷ÒøÒùÒúÒûÒüÒıÒşÓ¡Ø·Û´ÛÈÛóÜ§ÜáßÅà³áşâ¹ä¦äÎë³î÷ñ«ñ¿ò¾ö¯ö¸"};
static const unsigned char Pymb_ying[]  ={"Ó¢Ó£Ó¤Ó¥Ó¦Ó§Ó¨Ó©ÓªÓ«Ó¬Ó­Ó®Ó¯Ó°Ó±Ó²Ó³ÙøÛ«ÜãÜşİºİÓİöŞüàÓâßäŞäëå­çøè¬éºëôğĞñ¨ò£ó¿"};
static const unsigned char Pymb_yo[]    ={"Ó´Óıà¡"};
static const unsigned char Pymb_yong[]  ={"ÓµÓ¶Ó·Ó¸Ó¹ÓºÓ»Ó¼Ó½Ó¾Ó¿ÓÀÓÁÓÂÓÃÙ¸ÛÕÜ­à¯ã¼çßïŞğ®÷«÷Ó"};
static const unsigned char Pymb_you[]   ={"ÓÄÓÅÓÆÓÇÓÈÓÉÓÊÓËÓÌÓÍÓÎÓÏÓĞÓÑÓÒÓÓÓÔÓÕÓÖÓ×ØÕØüÙ§İ¬İ¯İµŞÌßÏàóå¶èÖéàë»îğğàòÄòÊòöòøôíöÏ÷î÷ø"};
static const unsigned char Pymb_yu[]    ={"ÎµÎ¾ÓØÓÙÓÚÓÛÓÜÓİÓŞÓßÓàÓáÓâÓãÓäÓåÓæÓçÓèÓéÓêÓëÓìÓíÓîÓïÓğÓñÓòÓóÓôÓõÓöÓ÷ÓøÓùÓúÓûÓüÓıÓşÔ¡Ô¢Ô£Ô¤Ô¥Ô¦Ø®Ø¹ØñÙ¶ÚÄÚÍİÇİÒİ÷ŞíàôàöáÎáüâÀâÅâ×ãĞå÷åıæ¥æúè¤êÅêìëéì£ì¶ìÏìÙìÛí±í²îÚğÁğÖğõğöñ¾ñÁòâòõóÄô§ô¨ö§ö¹"};
static const unsigned char Pymb_yuan[]  ={"Ô§Ô¨Ô©ÔªÔ«Ô¬Ô­Ô®Ô¯Ô°Ô±Ô²Ô³Ô´ÔµÔ¶Ô·Ô¸Ô¹ÔºÜ«Ü¾Şòà÷ãäæÂè¥éÚë¼íóğ°ó¢óîö½"};
static const unsigned char Pymb_yue[]   ={"ÀÖËµÔ»Ô¼Ô½Ô¾Ô¿ÔÀÔÁÔÂÔÃÔÄÙßßÜå®èİéĞë¾îá"};
static const unsigned char Pymb_yun[]   ={"Ô±ÔÅÔÆÔÇÔÈÔÉÔÊÔËÔÌÔÍÔÎÔÏÔĞÛ©Ü¿áñã¢ã³ç¡è¹éæêÀëµìÙóŞ"};

static const unsigned char Pymb_z[]     ={"ÔÚÕâÖĞÖ®Ö»×ö"};
static const unsigned char Pymb_za[]    ={"ÔÑÔÒÔÓÔúÕ¦ŞÙßÆ"};
static const unsigned char Pymb_zai[]   ={"ÔÔÔÕÔÖÔ×ÔØÔÙÔÚáÌçŞ"};
static const unsigned char Pymb_zan[]   ={"ÔÛÔÜÔİÔŞŞÙè¶êÃô¢ôØôõöÉ"};
static const unsigned char Pymb_zang[]  ={"²ØÔßÔàÔáæàê°"};
static const unsigned char Pymb_zao[]   ={"ÔâÔãÔäÔåÔæÔçÔèÔéÔêÔëÔìÔíÔîÔïßğ"};
static const unsigned char Pymb_ze[]    ={"ÔğÔñÔòÔóÕ¦ØÆØÓßõàıåÅê¾óĞóåô·"};
static const unsigned char Pymb_zei[]   ={"Ôô"};
static const unsigned char Pymb_zen[]   ={"ÔõÚÚ"};
static const unsigned char Pymb_zeng[]  ={"ÔöÔ÷ÔøÔùçÕêµîÀï­"};
static const unsigned char Pymb_zha[]   ={"²éÔúÔûÔüÔıÔşÕ¡Õ¢Õ£Õ¤Õ¥Õ¦Õ§Õ¨Õ©×õŞêß¸ßåßîà©é«íÄğäòÆö´÷ş"};
static const unsigned char Pymb_zhai[]  ={"µÔÔñÕªÕ«Õ¬Õ­Õ®Õ¯íÎñ©"};
static const unsigned char Pymb_zhan[]  ={"²üÕ°Õ±Õ²Õ³Õ´ÕµÕ¶Õ·Õ¸Õ¹ÕºÕ»Õ¼Õ½Õ¾Õ¿ÕÀÚŞŞøæöì¹"};
static const unsigned char Pymb_zhang[] ={"³¤ÕÁÕÂÕÃÕÄÕÅÕÆÕÇÕÈÕÉÕÊÕËÕÌÕÍÕÎÕÏØëÛµá¤áÖâ¯æÑè°ó¯"};
static const unsigned char Pymb_zhao[]  ={"³¯ÕĞÕÑÕÒÕÓÕÔÕÕÕÖÕ×ÕØÕÙ×¦×ÅÚ¯ßúèşîÈóÉ"};
static const unsigned char Pymb_zhe[]   ={"ÕÚÕÛÕÜÕİÕŞÕßÕàÕáÕâÕã×ÅÚØß¡èÏéüíİğÑñŞòØô÷"};
static const unsigned char Pymb_zhen[]  ={"ÕäÕåÕæÕçÕèÕéÕêÕëÕìÕíÕîÕïÕğÕñÕòÕóÖ¡ÛÚİèä¥äÚçÇèåé©é»éôêâëÓëŞìõî³ğ¡ğ²óğ"};
static const unsigned char Pymb_zheng[] ={"ÕôÕõÕöÕ÷ÕøÕùÕúÕûÕüÕıÕşÖ¢Ö£Ö¤Úºá¿áçîÛï£óİöë"};
static const unsigned char Pymb_zhi[]   ={"Ê¶Ö¥Ö¦Ö§Ö¨Ö©ÖªÖ«Ö¬Ö­Ö®Ö¯Ö°Ö±Ö²Ö³Ö´ÖµÖ¶Ö·Ö¸Ö¹ÖºÖ»Ö¼Ö½Ö¾Ö¿ÖÀÖÁÖÂÖÃÖÄÖÅÖÆÖÇÖÈÖÉÖÊÖËÖÌÖÍÖÎÖÏØ´ÚìÛ¤ÛúÜÆŞıàùáçâºâååéåëæïèÎè×èÙèäéòéùêŞëÕëùìíìóíéïôğºğëòÎôêõ¥õÅõÙõÜõôö£"};
static const unsigned char Pymb_zhong[] ={"ÖĞÖÑÖÒÖÓÖÔÖÕÖÖÖ×ÖØÖÙÖÚÚ£âìïñó®ô±õà"};
static const unsigned char Pymb_zhou[]  ={"ÖÛÖÜÖİÖŞÖßÖàÖáÖâÖãÖäÖåÖæÖçÖèİ§ßúæ¨æûç§ëĞíØô¦ôíôü"};
static const unsigned char Pymb_zhu[]   ={"ÊôÊõÖéÖêÖëÖìÖíÖîÖïÖğÖñÖòÖóÖôÖõÖöÖ÷ÖøÖùÖúÖûÖüÖıÖş×¡×¢×£×¤Ø¼ØùÙªÛ¥ÜÑÜïä¨ä¾äóèÌéÆéÍìÄîùğæğññÒóÃóçô¶ôãõî÷æ"};
static const unsigned char Pymb_zhua[]  ={"ÎÎ×¥×¦"};
static const unsigned char Pymb_zhuai[] ={"×§×ª"};
static const unsigned char Pymb_zhuan[] ={"´«×¨×©×ª×«×¬×­ßùâÍãçò§"};
static const unsigned char Pymb_zhuang[]={"´±×®×¯×°×±×²×³×´ŞÊí°"};
static const unsigned char Pymb_zhui[]  ={"×µ×¶×·×¸×¹×ºã·æíçÄö¿"};
static const unsigned char Pymb_zhun[]  ={"×»×¼â½ëÆñ¸"};
static const unsigned char Pymb_zhuo[]  ={"×½×¾×¿×À×Á×Â×Ã×Ä×Å×Æ×ÇÙ¾ÚÂßªä·äÃåªìÌìúí½ïí"};
static const unsigned char Pymb_zi[]    ={"Ö¨×È×É×Ê×Ë×Ì×Í×Î×Ï×Ğ×Ñ×Ò×Ó×Ô×Õ×ÖÚÑÜëáÑæ¢æÜç»è÷ê¢êßí§íöïÅïöñèóÊôÒôôõşö¤ö·öö÷Ú"};
static const unsigned char Pymb_zong[]  ={"×××Ø×Ù×Ú×Û×Ü×İÙÌèÈëêôÕ"};
static const unsigned char Pymb_zou[]   ={"×Ş×ß×à×áÚÁÚîÛ¸æãöí"};
static const unsigned char Pymb_zu[]    ={"×â×ã×ä×å×æ×ç×è×éÙŞİÏïß"};
static const unsigned char Pymb_zuan[]  ={"×ê×ëß¬çÚõò"};
static const unsigned char Pymb_zui[]   ={"×ì×í×î×ïŞ©õş"};
static const unsigned char Pymb_zun[]   ={"×ğ×ñß¤é×÷®"};
static const unsigned char Pymb_zuo[]   ={"´é×Á×ò×ó×ô×õ×ö×÷×ø×ùÚèßòàÜâôëÑìñíÄõ¡"};
static const unsigned char Pymb_space[] ={""};

/*"Æ´ÒôÊäÈë·¨²éÑ¯Âë±í,¶ş¼¶×ÖÄ¸Ë÷Òı±í(index)"*/
static const struct _PyIndex PyIndex_a[]={{"a",Pymb_a},
                                    {"ai",Pymb_ai},
                                    {"an",Pymb_an},
                                    {"ang",Pymb_ang},
                                    {"ao",Pymb_ao},};
static const struct _PyIndex PyIndex_b[]={{"b",Pymb_b},
                                    {"ba",Pymb_ba},
                                    {"bai",Pymb_bai},
                                    {"ban",Pymb_ban},
                                    {"bang",Pymb_bang},
                                    {"bao",Pymb_bao},
                                    {"bei",Pymb_bei},
                                    {"ben",Pymb_ben},
                                    {"beng",Pymb_beng},
                                    {"bi",Pymb_bi},
                                    {"bian",Pymb_bian},
                                    {"biao",Pymb_biao},
                                    {"bie",Pymb_bie},
                                    {"bin",Pymb_bin},
                                    {"bing",Pymb_bing},
                                    {"bo",Pymb_bo},
                                    {"bu",Pymb_bu}};
static const struct _PyIndex PyIndex_c[]={{"c",Pymb_c},
                                    {"ca",Pymb_ca},
                                    {"cai",Pymb_cai},
                                    {"can",Pymb_can},
                                    {"cang",Pymb_cang},
                                    {"cao",Pymb_cao},
                                    {"ce",Pymb_ce},
                                    {"cen",Pymb_cen},
                                    {"ceng",Pymb_ceng},
                                    {"cha",Pymb_cha},
                                    {"chai",Pymb_chai},
                                    {"chan",Pymb_chan},
                                    {"chang",Pymb_chang},
                                    {"chao",Pymb_chao},
                                    {"che",Pymb_che},
                                    {"chen",Pymb_chen},
                                    {"cheng",Pymb_cheng},
                                    {"chi",Pymb_chi},
                                    {"chong",Pymb_chong},
                                    {"chou",Pymb_chou},
                                    {"chu",Pymb_chu},
                                    {"chuai",Pymb_chuai},
                                    {"chuan",Pymb_chuan},
                                    {"chuang",Pymb_chuang},
                                    {"chui",Pymb_chui},
                                    {"chun",Pymb_chun},
                                    {"chuo",Pymb_chuo},
                                    {"ci",Pymb_ci},
                                    {"cong",Pymb_cong},
                                    {"cou",Pymb_cou},
                                    {"cu",Pymb_cu},
                                    {"cuan",Pymb_cuan},
                                    {"cui",Pymb_cui},
                                    {"cun",Pymb_cun},
                                    {"cuo",Pymb_cuo}};
static const struct _PyIndex PyIndex_d[]={{"d",Pymb_d},
                                    {"da",Pymb_da},
                                    {"dai",Pymb_dai},
                                    {"dan",Pymb_dan},
                                    {"dang",Pymb_dang},
                                    {"dao",Pymb_dao},
                                    {"de",Pymb_de},
                                    {"deng",Pymb_deng},
                                    {"di",Pymb_di},
                                    {"dia",Pymb_dia},
                                    {"dian",Pymb_dian},
                                    {"diao",Pymb_diao},
                                    {"die",Pymb_die},
                                    {"ding",Pymb_ding},
                                    {"diu",Pymb_diu},
                                    {"dong",Pymb_dong},
                                    {"dou",Pymb_dou},
                                    {"du",Pymb_du},
                                    {"duan",Pymb_duan},
                                    {"dui",Pymb_dui},
                                    {"dun",Pymb_dun},
                                    {"duo",Pymb_duo}};
static const struct _PyIndex PyIndex_e[]={{"e",Pymb_e},
                                    {"ei",Pymb_ei},
                                    {"en",Pymb_en},
                                    {"er",Pymb_er}};
static const struct _PyIndex PyIndex_f[]={{"f",Pymb_f},
                                    {"fa",Pymb_fa},
                                    {"fan",Pymb_fan},
                                    {"fang",Pymb_fang},
                                    {"fei",Pymb_fei},
                                    {"fen",Pymb_fen},
                                    {"feng",Pymb_feng},
                                    {"fo",Pymb_fo},
                                    {"fou",Pymb_fou},
                                    {"fu",Pymb_fu}};
static const struct _PyIndex PyIndex_g[]={{"g",Pymb_g},
                                    {"ga",Pymb_ga},
                                    {"gai",Pymb_gai},
                                    {"gan",Pymb_gan},
                                    {"gang",Pymb_gang},
                                    {"gao",Pymb_gao},
                                    {"ge",Pymb_ge},
                                    {"gei",Pymb_gei},
                                    {"gen",Pymb_gen},
                                    {"geng",Pymb_geng},
                                    {"gong",Pymb_gong},
                                    {"gou",Pymb_gou},
                                    {"gu",Pymb_gu},
                                    {"gua",Pymb_gua},
                                    {"guai",Pymb_guai},
                                    {"guan",Pymb_guan},
                                    {"guang",Pymb_guang},
                                    {"gui",Pymb_gui},
                                    {"gun",Pymb_gun},
                                    {"guo",Pymb_guo}};
static const struct _PyIndex PyIndex_h[]={{"h",Pymb_h},
                                    {"ha",Pymb_ha},
                                    {"hai",Pymb_hai},
                                    {"han",Pymb_han},
                                    {"hang",Pymb_hang},
                                    {"hao",Pymb_hao},
                                    {"he",Pymb_he},
                                    {"hei",Pymb_hei},
                                    {"hen",Pymb_hen},
                                    {"heng",Pymb_heng},
                                    {"hong",Pymb_hong},
                                    {"hou",Pymb_hou},
                                    {"hu",Pymb_hu},
                                    {"hua",Pymb_hua},
                                    {"huai",Pymb_huai},
                                    {"huan",Pymb_huan},
                                    {"huang ",Pymb_huang},
                                    {"hui",Pymb_hui},
                                    {"hun",Pymb_hun},
                                    {"huo",Pymb_huo}};
//static const struct _PyIndex PyIndex_i[]={{"i",Pymb_space}};
static const struct _PyIndex PyIndex_j[]={{"j",Pymb_j},
                                    {"ji",Pymb_ji},
                                    {"jia",Pymb_jia},
                                    {"jian",Pymb_jian},
                                    {"jiang",Pymb_jiang},
                                    {"jiao",Pymb_jiao},
                                    {"jie",Pymb_jie},
                                    {"jin",Pymb_jin},
                                    {"jing",Pymb_jing},
                                    {"jiong",Pymb_jiong},
                                    {"jiu",Pymb_jiu},
                                    {"ju",Pymb_ju},
                                    {"juan",Pymb_juan},
                                    {"jue",Pymb_jue},
                                    {"jun",Pymb_jun}};
static const struct _PyIndex PyIndex_k[]={{"k",Pymb_k},
                                    {"ka",Pymb_ka},
                                    {"kai",Pymb_kai},
                                    {"kan",Pymb_kan},
                                    {"kang",Pymb_kang},
                                    {"kao",Pymb_kao},
                                    {"ke",Pymb_ke},
                                    {"ken",Pymb_ken},
                                    {"keng",Pymb_keng},
                                    {"kong",Pymb_kong},
                                    {"kou",Pymb_kou},
                                    {"ku",Pymb_ku},
                                    {"kua",Pymb_kua},
                                    {"kuai",Pymb_kuai},
                                    {"kuan",Pymb_kuan},
                                    {"kuang",Pymb_kuang},
                                    {"kui",Pymb_kui},
                                    {"kun",Pymb_kun},
                                    {"kuo",Pymb_kuo}};
static const struct _PyIndex PyIndex_l[]={{"l",Pymb_l},
                                    {"la",Pymb_la},
                                    {"lai",Pymb_lai},
                                    {"lan",Pymb_lan},
                                    {"lang",Pymb_lang},
                                    {"lao",Pymb_lao},
                                    {"le",Pymb_le},
                                    {"lei",Pymb_lei},
                                    {"leng",Pymb_leng},
                                    {"li",Pymb_li},
                                    {"lian",Pymb_lian},
                                    {"liang",Pymb_liang},
                                    {"liao",Pymb_liao},
                                    {"lie",Pymb_lie},
                                    {"lin",Pymb_lin},
                                    {"ling",Pymb_ling},
                                    {"liu",Pymb_liu},
                                    {"lo",Pymb_lo},
                                    {"long",Pymb_long},
                                    {"lou",Pymb_lou},
                                    {"lu",Pymb_lu},
                                    {"luan",Pymb_luan},
                                    {"lue",Pymb_lue},
                                    {"lun",Pymb_lun},
                                    {"luo",Pymb_luo},
                                    {"lv",Pymb_lv}};
static const struct _PyIndex PyIndex_m[]={{"m",Pymb_m},
                                    {"ma",Pymb_ma},
                                    {"mai",Pymb_mai},
                                    {"man",Pymb_man},
                                    {"mang",Pymb_mang},
                                    {"mao",Pymb_mao},
                                    {"me",Pymb_me},
                                    {"mei",Pymb_mei},
                                    {"men",Pymb_men},
                                    {"meng",Pymb_meng},
                                    {"mi",Pymb_mi},
                                    {"mian",Pymb_mian},
                                    {"miao",Pymb_miao},
                                    {"mie",Pymb_mie},
                                    {"min",Pymb_min},
                                    {"ming",Pymb_ming},
                                    {"miu",Pymb_miu},
                                    {"mo",Pymb_mo},
                                    {"mou",Pymb_mou},
                                    {"mu",Pymb_mu}};
static const struct _PyIndex PyIndex_n[]={{"n",Pymb_n},
                                    {"na",Pymb_na},
                                    {"nai",Pymb_nai},
                                    {"nan",Pymb_nan},
                                    {"nang",Pymb_nang},
                                    {"nao",Pymb_nao},
                                    {"ne",Pymb_ne},
                                    {"nei",Pymb_nei},
                                    {"nen",Pymb_nen},
                                    {"neng",Pymb_neng},
                                    {"ni",Pymb_ni},
                                    {"nian",Pymb_nian},
                                    {"niang",Pymb_niang},
                                    {"niao",Pymb_niao},
                                    {"nie",Pymb_nie},
                                    {"nin",Pymb_nin},
                                    {"ning",Pymb_ning},
                                    {"niu",Pymb_niu},
                                    {"nou",Pymb_nou},
                                    {"nong",Pymb_nong},
                                    {"nu",Pymb_nu},
                                    {"nuan",Pymb_nuan},
                                    {"nue",Pymb_nue},
                                    {"nuo",Pymb_nuo},
                                    {"nv",Pymb_nv}};
static const struct _PyIndex PyIndex_o[]={{"o",Pymb_o},
                                    {"ou",Pymb_ou}};
static const struct _PyIndex PyIndex_p[]={{"p",Pymb_p},
                                    {"pa",Pymb_pa},
                                    {"pai",Pymb_pai},
                                    {"pan",Pymb_pan},
                                    {"pang",Pymb_pang},
                                    {"pao",Pymb_pao},
                                    {"pei",Pymb_pei},
                                    {"pen",Pymb_pen},
                                    {"peng",Pymb_peng},
                                    {"pi",Pymb_pi},
                                    {"pian",Pymb_pian},
                                    {"piao",Pymb_piao},
                                    {"pie",Pymb_pie},
                                    {"pin",Pymb_pin},
                                    {"ping",Pymb_ping},
                                    {"po",Pymb_po},
                                    {"pou",Pymb_pou},
                                    {"pu",Pymb_pu}};
static const struct _PyIndex PyIndex_q[]={{"q",Pymb_q},
                                    {"qi",Pymb_qi},
                                    {"qia",Pymb_qia},
                                    {"qian",Pymb_qian},
                                    {"qiang",Pymb_qiang},
                                    {"qiao",Pymb_qiao},
                                    {"qie",Pymb_qie},
                                    {"qin",Pymb_qin},
                                    {"qing",Pymb_qing},
                                    {"qiong",Pymb_qiong},
                                    {"qiu",Pymb_qiu},
                                    {"qu",Pymb_qu},
                                    {"quan",Pymb_quan},
                                    {"que",Pymb_que},
                                    {"qun",Pymb_qun}};
static const struct _PyIndex PyIndex_r[]={{"r",Pymb_r},
                                    {"ran",Pymb_ran},
                                    {"rang",Pymb_rang},
                                    {"rao",Pymb_rao},
                                    {"re",Pymb_re},
                                    {"ren",Pymb_ren},
                                    {"reng",Pymb_reng},
                                    {"ri",Pymb_ri},
                                    {"rong",Pymb_rong},
                                    {"rou",Pymb_rou},
                                    {"ru",Pymb_ru},
                                    {"ruan",Pymb_ruan},
                                    {"rui",Pymb_rui},
                                    {"run",Pymb_run},
                                    {"ruo",Pymb_ruo}};
static const struct _PyIndex PyIndex_s[]={{"s",Pymb_s},
                                    {"sa",Pymb_sa},
                                    {"sai",Pymb_sai},
                                    {"san",Pymb_san},
                                    {"sang",Pymb_sang},
                                    {"sao",Pymb_sao},
                                    {"se",Pymb_se},
                                    {"sen",Pymb_sen},
                                    {"seng",Pymb_seng},
                                    {"sha",Pymb_sha},
                                    {"shai",Pymb_shai},
                                    {"shan",Pymb_shan},
                                    {"shang ",Pymb_shang},
                                    {"shao",Pymb_shao},
                                    {"she",Pymb_she},
                                    {"shen",Pymb_shen},
                                    {"sheng",Pymb_sheng},
                                    {"shi",Pymb_shi},
                                    {"shou",Pymb_shou},
                                    {"shu",Pymb_shu},
                                    {"shua",Pymb_shua},
                                    {"shuai",Pymb_shuai},
                                    {"shuan",Pymb_shuan},
                                    {"shuang",Pymb_shuang},
                                    {"shui",Pymb_shui},
                                    {"shun",Pymb_shun},
                                    {"shuo",Pymb_shuo},
                                    {"si",Pymb_si},
                                    {"song",Pymb_song},
                                    {"sou",Pymb_sou},
                                    {"su",Pymb_su},
                                    {"suan",Pymb_suan},
                                    {"sui",Pymb_sui},
                                    {"sun",Pymb_sun},
                                    {"suo",Pymb_suo}};
static const struct _PyIndex PyIndex_t[]={{"t",Pymb_t},
                                    {"ta",Pymb_ta},
                                    {"tai",Pymb_tai},
                                    {"tan",Pymb_tan},
                                    {"tang",Pymb_tang},
                                    {"tao",Pymb_tao},
                                    {"te",Pymb_te},
                                    {"teng",Pymb_teng},
                                    {"ti",Pymb_ti},
                                    {"tian",Pymb_tian},
                                    {"tiao",Pymb_tiao},
                                    {"tie",Pymb_tie},
                                    {"ting",Pymb_ting},
                                    {"tong",Pymb_tong},
                                    {"tou",Pymb_tou},
                                    {"tu",Pymb_tu},
                                    {"tuan",Pymb_tuan},
                                    {"tui",Pymb_tui},
                                    {"tun",Pymb_tun},
                                    {"tuo",Pymb_tuo}};
//static const struct _PyIndex PyIndex_u[]={{"u",Pymb_space}};
//static const struct _PyIndex PyIndex_v[]={{"v",Pymb_space}};
static const struct _PyIndex PyIndex_w[]={{"w",Pymb_w},
                                    {"wa",Pymb_wa},
                                    {"wai",Pymb_wai},
                                    {"wan",Pymb_wan},
                                    {"wang",Pymb_wang},
                                    {"wei",Pymb_wei},
                                    {"wen",Pymb_wen},
                                    {"weng",Pymb_weng},
                                    {"wo",Pymb_wo},
                                    {"wu",Pymb_wu}};
static const struct _PyIndex PyIndex_x[]={{"x",Pymb_x},
                                    {"xi",Pymb_xi},
                                    {"xia",Pymb_xia},
                                    {"xian",Pymb_xian},
                                    {"xiang",Pymb_xiang},
                                    {"xiao",Pymb_xiao},
                                    {"xie",Pymb_xie},
                                    {"xin",Pymb_xin},
                                    {"xing",Pymb_xing},
                                    {"xiong",Pymb_xiong},
                                    {"xiu",Pymb_xiu},
                                    {"xu",Pymb_xu},
                                    {"xuan",Pymb_xuan},
                                    {"xue",Pymb_xue},
                                    {"xun",Pymb_xun}};
static const struct _PyIndex PyIndex_y[]={ {"y",Pymb_y},
                                    {"ya",Pymb_ya},
                                    {"yan",Pymb_yan},
                                    {"yang",Pymb_yang},
                                    {"yao",Pymb_yao},
                                    {"ye",Pymb_ye},
                                    {"yi",Pymb_yi},
                                    {"yin",Pymb_yin},
                                    {"ying",Pymb_ying},
                                    {"yo",Pymb_yo},
                                    {"yong",Pymb_yong},
                                    {"you",Pymb_you},
                                    {"yu",Pymb_yu},
                                    {"yuan",Pymb_yuan},
                                    {"yue",Pymb_yue},
                                    {"yun",Pymb_yun}};
static const struct _PyIndex PyIndex_z[]={{"z",Pymb_z},
                                    {"za",Pymb_za},
                                    {"zai",Pymb_zai},
                                    {"zan",Pymb_zan},
                                    {"zang",Pymb_zang},
                                    {"zao",Pymb_zao},
                                    {"ze",Pymb_ze},
                                    {"zei",Pymb_zei},
                                    {"zen",Pymb_zen},
                                    {"zeng",Pymb_zeng},
                                    {"zha",Pymb_zha},
                                    {"zhai",Pymb_zhai},
                                    {"zhan",Pymb_zhan},
                                    {"zhang",Pymb_zhang},
                                    {"zhao",Pymb_zhao},
                                    {"zhe",Pymb_zhe},
                                    {"zhen",Pymb_zhen},
                                    {"zheng",Pymb_zheng},
                                    {"zhi",Pymb_zhi},
                                    {"zhong",Pymb_zhong},
                                    {"zhou",Pymb_zhou},
                                    {"zhu",Pymb_zhu},
                                    {"zhua",Pymb_zhua},
                                    {"zhuai",Pymb_zhuai},
                                    {"zhuan",Pymb_zhuan},
                                    {"zhuang",Pymb_zhuang},
                                    {"zhui",Pymb_zhui},
                                    {"zhun",Pymb_zhun},
                                    {"zhuo",Pymb_zhuo},
                                    {"zi",Pymb_zi},
                                    {"zong",Pymb_zong},
                                    {"zou",Pymb_zou},
                                    {"zu",Pymb_zu},
                                    {"zuan",Pymb_zuan},
                                    {"zui",Pymb_zui},
                                    {"zun",Pymb_zun},
                                    {"zuo",Pymb_zuo}};
const struct _PyIndex PyIndex_end[]={{"",Pymb_space}};

/*¶¨ÒåÊ××ÖÄ¸Ë÷Òı±í*/
const struct _PyIndex *const PyIndex_headletter[]={PyIndex_a,
                                            PyIndex_b,
                                            PyIndex_c,
                                            PyIndex_d,
                                            PyIndex_e,
                                            PyIndex_f,
                                            PyIndex_g,
                                            PyIndex_h,
                                            NULL,//PyIndex_i,
                                            PyIndex_j,
                                            PyIndex_k,
                                            PyIndex_l,
                                            PyIndex_m,
                                            PyIndex_n,
                                            PyIndex_o,
                                            PyIndex_p,
                                            PyIndex_q,
                                            PyIndex_r,
                                            PyIndex_s,
                                            PyIndex_t,
                                            NULL,//PyIndex_u,
                                            NULL,//PyIndex_v,
                                            PyIndex_w,
                                            PyIndex_x,
                                            PyIndex_y,
                                            PyIndex_z,
                                            PyIndex_end};

const unsigned char *Pyime(unsigned char *strInput_Pystr)
{
    const struct _PyIndex *cpHZ,*cpHZedge;
    unsigned char i,cInputStrLength;

    cInputStrLength=strlen((const char*)strInput_Pystr);    /*ÊäÈëÆ´Òô´®³¤¶È*/
    if(*strInput_Pystr=='\0')return("");       /*Èç¹ûÊäÈë¿Õ×Ö·û·µ»Ø0*/

    for(i=0;i<cInputStrLength;i++)
        *(strInput_Pystr+i)|=0x20;             /*½«×ÖÄ¸´®×ªÎªĞ¡Ğ´*/

    if(*strInput_Pystr=='i')return("");        /*´íÎóÆ´ÒôÊäÈë*/
    if(*strInput_Pystr=='u')return("");
    if(*strInput_Pystr=='v')return("");

    cpHZ=PyIndex_headletter[strInput_Pystr[0]-'a'];        /*²éÊ××ÖÄ¸Ë÷Òı*/
    cpHZedge=PyIndex_headletter[strInput_Pystr[0]-'a'+1];  /*ÉèÖÃÖ¸Õë½çÏŞ*/

    strInput_Pystr++;                           /*Ö¸ÏòÆ´Òô´®µÚ¶ş¸ö×ÖÄ¸*/
    while(cpHZ<cpHZedge)                         /*Ë÷Òı±í²»³¬½ç*/
    {
        for(i=0;i<cInputStrLength;i++)
        {
            if(*(strInput_Pystr+i)!=*((*cpHZ).Py+i))break;    /*·¢ÏÖ×ÖÄ¸´®²»Åä,ÍË³ö*/
        }
        if(i==cInputStrLength)        /*×ÖÄ¸´®È«Åä*/
        {
            return (*cpHZ).Pymb;
        }
        cpHZ++;
    }
    return "";                      /*ÎŞ¹û¶øÖÕ*/
}

/*************************************************************************

                            Êı×Ö¼üÆ´ÒôÊäÈë·¨

*************************************************************************/
//Î¬Ò»:×ªÒåºóµÄ×Ö£¬Î¬¶ş:Êı1:ÆğÊ¼×ÖÄ¸,Î¬¶ş:¸ÃÊı×Ö¼ü¶ÔÓ¦×ÖÄ¸¸öÊı
static const unsigned char Num2Letter[8][2] = {
   'a',3,/*2*/   'd',3,/*3*/  'g',3, /*4*/  'j',3,/*5*/
   'm',3,/*6*/   'p',4,/*7*/  't',3, /*8*/  'w',4/*9*/
};

//------------------------------°´¼üÏìÓ¦³ÌĞò--------------------------------
//°´¼üÊ±µ÷ÓÃ´Ëº¯Êı
//·µ»ØÕÒµ½µÄ×ÖÄ¸±í
//Èô·µ»Ø¿Õ,±íÊ¾´Ë´ÎÃ»ÕÒµ½
__arm const struct _PyIndex **NumKeyPyIme_pKey(PinYinIME_t *pPy,
                                         unsigned char *pFindCount,//·µ»ØÕÒµ½µÄ¹ıÊı      
                                         unsigned char Num)   //ÊäÈëµÄ°´¼üÖµ,ÏìÓ¦2-9,ÍË¸ñ¼ü
{
  
  unsigned char NumCount = pPy->NumCount;
  unsigned char PrvFindCount = pPy->PerFindCount[NumCount - 1];//ÉÏ´ÎÕÒµ½¸öÊı
  const struct _PyIndex **pPyTblIndex = pPy->pPyTblIndex + pPy->CurPos;//ÉÏ´ÎÎ»ÖÃ
  //¼ì²éÊÇ·ñÊÇÍË¸ñ¼ü,ÈôÊÇÔò¼õÉÙÒ»¸öÊı×Ö,Ö¸ÏòÉÏÉÏ´ÎÎ»ÖÃ
  if(Num == ' '){
    if(NumCount >= 2){ //ÊäÈëÁËÁ½¸ö×ÖÄ¸Ê±,ÉÏÒ»¸ö²ÅÓĞ
      NumCount--;
      pPy->NumCount = NumCount;
      PrvFindCount = pPy->PerFindCount[NumCount - 1];//ÉÏÉÏ´ÎµÄÕÒµ½¸öÊı
      pPy->PerFindCount[NumCount] = 0;  //ĞèÖØĞÂ¿ªÊ¼ÕÒ
      *pFindCount = PrvFindCount;//ÉÏ´Î¸öÊı
      pPy->CurPos -= PrvFindCount;
      return pPyTblIndex -= PrvFindCount;  //·µ»ØÉÏ´Î²éµ½µÄÎ»ÖÃµÄÖµ
    }
    else{//½öÊäÈëÒ»¸ö×ÖÄ¸»ò´ÓÎ´ÊäÈëÊ±
       pPy->NumCount = 0;
       return NULL; //Çå¿ÕÁË
    }
  }
  
  //¼üÖµ°´´íÊ±
  if((Num < '2') || (Num > '9')){
    if(NumCount){
     *pFindCount = PrvFindCount;
     return pPyTblIndex;  //±¾´ÎÎ»ÖÃ²»±ä
    }
   else return NULL;  //»¹Ã»ÓĞÊäÈë¹ıÄØ
  }
  
  unsigned char Letter = Num2Letter[Num - '2'][0];
  unsigned char LetterCount = Num2Letter[Num - '2'][1];
  //=====================ÊäÈëµÚÒ»¸ö×ÖÄ¸Ê±,·µ»ØÊ×¸ö×Ö=========================
  if(!NumCount){
    pPyTblIndex = pPy->pPyTblIndex;
    unsigned char Unused = 0;
    Letter -= 'a';
    for(unsigned char i = 0; i < LetterCount; i++){
      const struct _PyIndex *pCurPyIndex = PyIndex_headletter[Letter++];
      if(pCurPyIndex != NULL) *pPyTblIndex++ = pCurPyIndex;
      else Unused++;
    }
    *pFindCount = LetterCount - Unused;
    pPy->PerFindCount[0] = LetterCount - Unused;
    pPy->CurPos = 0;
    pPy->NumCount += 1;    //°´¼üÕıÈ·
    return pPyTblIndex;
  }
  //============================ÊäÈëÊı×Ö¹ı¶àÊ±============================
  else if(NumCount > 6){
     *pFindCount = PrvFindCount;
     return pPyTblIndex;  //±¾´ÎÎ»ÖÃ²»±ä
  }
  
  //========================ÔÙÊäÈëÊ±,ÔÚÉÏ´ÎµÄ»ù´¡ÉÏ¼ÌĞø²éÑ¯±í===============
  unsigned char PrvLetterPos = NumCount - 1; //ÉÏ´Î×ÖÄ¸ËùÔÚÎ»ÖÃ
  
  const struct _PyIndex **pPyTblNewIndex = //ĞÂÕÒµ½Ôö¼ÓµÄÏîÎ»ÖÃ,´ËÊ±NumCount¶ÔÆëĞÂË÷ÒıÖµ
    pPyTblIndex + pPy->PerFindCount[PrvLetterPos];  
  
  //ÔÚÉÏ´Î»ù´¡ÉÏ²éÕÒ,°´Æ´Òô×ÖÄ¸±íµÄÅÅĞò·½Ê½½øĞĞ²Ù×÷
  unsigned char CurFindCount = 0; //ÕÒµ½¸öÊı
  for(unsigned char j = 0; j < PrvFindCount; j++){
    const struct _PyIndex *pPyCurIndex = *(pPyTblIndex + j); //ÉÏ´ÎÎ»ÖÃ³õÊ¼»¯
    unsigned char PrvLetter = *(pPyCurIndex->Py + PrvLetterPos);//ÉÏ´ÎµÄ×ÖÄ¸
    //ÉÏ´ÎÍêÈ«·ûºÏÊ±ÉÏ´Î¾Í²»¼ì²éÉÏ×ÖÁË
    if(*(pPyCurIndex->Py + NumCount) == '\0')
       pPyCurIndex++; 
   //ÔÚµ±Ç°Î»ÖÃÖĞ½«×Ö·ûÎ»ÖÃÒÆ¶¯µ½±¾´ÎÊ××ÖÎ»ÖÃ
    while(*(pPyCurIndex->Py + PrvLetterPos) == PrvLetter){
      if(*(pPyCurIndex->Py + NumCount) >= Letter) break;
      pPyCurIndex++; 
    }
   //ÔÚ±¾´ÎÊı×Ö¶ÔÓ¦µÄ×Ö·û´®ÏòÇ°²éÕÒ
   for(unsigned char CurLetter = Letter; 
       CurLetter < (Letter + LetterCount); CurLetter++){
     const unsigned char *pString = pPyCurIndex->Py; 
     if(*(pString + PrvLetterPos) != PrvLetter) break;//ÉÏ´Î×Ö¾Í²»·ûºÏ£¬Ö±½Óµ½±ß½çÁË
     int Find1stFlag = 0;  //ÕÒµ½Ê××Ö±êÖ¾
     while(*(pString + NumCount) == CurLetter){//±¾´Î¶ÔÓ¦Ê±,½«±¾´Î·ûºÏµÄ²éÕÒÍê
       if(!Find1stFlag){//ÕÒµ½µÚÒ»¸öÊ±
          CurFindCount++;
          *pPyTblNewIndex++ = pPyCurIndex; //ÕÒµ½³¬¹ıÒ»¸ö,ÍùºóÔö¼Ó
          Find1stFlag = -1;   //±¾´ÎÕÒµ½
        }
        pPyCurIndex++;
        pString = pPyCurIndex->Py;
     }
   }//end for i;    
  }//end for j;
  
  if(CurFindCount){//ÓĞÕÒµ½Ê±
    *pFindCount = CurFindCount;   //ÕÒµ½¸öÊı
    pPy->PerFindCount[NumCount] = CurFindCount;
    pPy->CurPos += PrvFindCount;
    pPy->NumCount++;
    return pPyTblIndex + PrvFindCount;
  }
  //·µ»ØÉÏ´Î²ÎÊıÏà¹Ø
  *pFindCount = PrvFindCount;
  return pPyTblIndex;  //±¾´ÎÎ»ÖÃ²»±ä
}

//------------------------------µÃµ½µ±Ç°×ÖÄ¸±í--------------------------------
//·µ»ØÕÒµ½µÄ×ÖÄ¸±í
const struct _PyIndex **NumKeyPyIme_pGetCur(PinYinIME_t *pPy,
                                         unsigned char *pFindCount)//·µ»ØÕÒµ½µÄ¹ıÊı 
{
  *pFindCount = pPy->PerFindCount[pPy->NumCount - 1];
  return &(pPy->pPyTblIndex[pPy->CurPos]);
}

