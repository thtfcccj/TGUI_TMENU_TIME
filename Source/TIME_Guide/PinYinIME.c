/*
           pyInput.c - Æ´ÒôÊäÈë·¨Ä£¿é

           Ô­×÷:ÕÅ ¿­
           ¸ÄĞ´:Àî Ç¿(mail2li@21cn.com)
           ÖØ¸ÄĞ´£ºdap_ccj
*/

#include <string.h>
#include <stdio.h>

#include "PinYinIME.h"

/* "Æ´ÒôÊäÈë·¨ºº×ÖÅÅÁĞ±í,Âë±í(mb)" */
const unsigned char Pymb_a[]     ={"°¡°¢ëç"};
const unsigned char Pymb_ai[]    ={"°®°«°¤°¥°­°©°¬°¦°§°ª°¯°£°¨´ô"};
const unsigned char Pymb_an[]    ={"°´°²°µ°¶°³°¸°°°±°·÷ö"};
const unsigned char Pymb_ang[]   ={"°º°¹°»"};
const unsigned char Pymb_ao[]    ={"°À°¼°Á°Â°¾°Ã°½°¿°ÄÛêæñéá"};
const unsigned char Pymb_b[]     ={"°É°Ë²»±¾±È°Ñ°×"};
const unsigned char Pymb_ba[]    ={"°Ñ°Ë°É°Ö°Î°Õ°Ï°Í°Å°Ç°Ó°Ô°È°Ğ°Ê°Ì°Æ°Ò"};
const unsigned char Pymb_bai[]   ={"°Ù°×°Ú°Ü°Ø°İ°Û°Ù²®°ŞêşŞã"};
const unsigned char Pymb_ban[]   ={"°ë°ì°à°ã°è°á°æ°ß°å°é°â°ç°ê°ä°íÚæÛàîÓ"};
const unsigned char Pymb_bang[]  ={"°ï°ô°ó°õ°î°ğ°ñ°ò°ö°ø°ù°õ°÷äº"};
const unsigned char Pymb_bao[]   ={"°ü±§±¨±¥±£±©±¡±¦±¬°ş±ªÅÙ°ú°û°ı±¢±¤±«ÆÙİáæßìÒñÙ"};
const unsigned char Pymb_bei[]   ={"±»±±±¶±­±³±¯±¸±®±°±´±²±µ±º±·±¹"};
const unsigned char Pymb_ben[]   ={"±¾±¼±½±¿º»"};
const unsigned char Pymb_beng[]  ={"±Ä±Á±Â±Å±À±Ã"};
const unsigned char Pymb_bi[]    ={"±È±Ê±Õ±Ç±Ì±Ò±Ø±Ü±Æ±Ï±Û±Ë±É±Ó±Ñ±İ±Ğ±Ö±Ô±Í±×±Î±ÚØ°"};
const unsigned char Pymb_bian[]  ={"±ß±ä±ã±é±à±Ş±á±â±å±æ±ç±è"};
const unsigned char Pymb_biao[]  ={"±í±ê±ë±ìæ»ñÑ"};
const unsigned char Pymb_bie[]   ={"±ğ±ï±î±ñ"};
const unsigned char Pymb_bin[]   ={"±ö±ò±ó±õ±ô±÷"};
const unsigned char Pymb_bing[]  ={"±ù±ø±û²¢²¡±ı±ü±ú±ş"};
const unsigned char Pymb_bo[]    ={"²¦²¨²¥²£²§²±²¤²®²µ²¯²´²ª²¬²°²©²³²«²­²²²·"};
const unsigned char Pymb_bu[]    ={"²»²¼²½²¿²¹²¸²¶²À²º²¾"};
const unsigned char Pymb_c[]     ={"²Å´Î³¤³ö²à´Ó"};
const unsigned char Pymb_ca[]    ={"²Áàê"};
const unsigned char Pymb_cai[]   ={"²Â²Å²Ä²Æ²Ã²É²Ê²Ç²È²Ë²Ì"};
const unsigned char Pymb_can[]   ={"²Î²Í²Ğ²Ï²Ñ²Ò²Ó"};
const unsigned char Pymb_cang[]  ={"²Ö²×²Ô²Õ²Ø"};
const unsigned char Pymb_cao[]   ={"²Ù²Ú²Ü²Û²İ"};
const unsigned char Pymb_ce[]    ={"²á²à²Ş²â²ß"};
const unsigned char Pymb_ceng[]  ={"²ã²äÔø"};
const unsigned char Pymb_cha[]   ={"²î²æ²å²é²ç²è²ë²ì²ê²í²ïÉ²"};
const unsigned char Pymb_chai[]  ={"²ğ²ñ²ò"};
const unsigned char Pymb_chan[]  ={"²ô²ó²÷²ö²ø²õ²ú²ù²û²ü"};
const unsigned char Pymb_chang[] ={"²ı³¦³¢³¥³£³§³¡³¨³©²ş³«³ª"};
const unsigned char Pymb_chao[]  ={"³­³®³¬³²³¯³°³±³³³´´Â"};
const unsigned char Pymb_che[]   ={"³µ³¶³¹³¸³·³º"};
const unsigned char Pymb_chen[]  ={"³»³¾³¼³À³Á³½³Â³¿³Ä³Ã"};
const unsigned char Pymb_cheng[] ={"³Æ³Å³É³Ê³Ğ³Ï³Ç³Ë³Í³Ì³Î³È³Ñ³Ò³Ó"};
const unsigned char Pymb_chi[]   ={"³Ô³Õ³Ú³Ø³Û³Ù³Ö³ß³Ş³İ³Ü³â³à³ã³á"};
const unsigned char Pymb_chong[] ={"³ä³å³æ³ç³è"};
const unsigned char Pymb_chou[]  ={"³é³ğ³ñ³ë³î³í³ï³ê³ì³ó³ò³ô"};
const unsigned char Pymb_chu[]   ={"³ö³õ³ı³ø³ü³ú³û³÷³ù´¡´¢³ş´¦´¤´¥´£Ğó"};
const unsigned char Pymb_chuai[] ={"´§"};
const unsigned char Pymb_chuan[] ={"´¨´©´«´¬´ª´­´®"};
const unsigned char Pymb_chuang[]={"´³´¯´°´²´´"};
const unsigned char Pymb_chui[]  ={"´µ´¶´¹´·´¸"};
const unsigned char Pymb_chun[]  ={"´º´»´¿´½´¾´¼´À"};
const unsigned char Pymb_chuo[]  ={"´Á"};
const unsigned char Pymb_ci[]    ={"´Ã´Ê´Ä´É´È´Ç´Å´Æ´Ë´Î´Ì´Í"};
const unsigned char Pymb_cong[]  ={"´Ñ´Ó´Ò´Ğ´Ï´Ô"};
const unsigned char Pymb_cou[]   ={"´Õ"};
const unsigned char Pymb_cu[]    ={"´Ö´Ù´×´Ø"};
const unsigned char Pymb_cuan[]  ={"´Ú´Ü´Û"};
const unsigned char Pymb_cui[]   ={"´Ş´ß´İ´à´ã´á´â´ä"};
const unsigned char Pymb_cun[]   ={"´å´æ´ç"};
const unsigned char Pymb_cuo[]   ={"´ê´è´é´ì´ë´í"};
const unsigned char Pymb_d[]     ={"µÄµãµÈ¶àµ½´ó"};
const unsigned char Pymb_da[]    ={"´î´ï´ğ´ñ´ò´ó"};
const unsigned char Pymb_dai[]   ={"´ô´õ´ö´ú´ø´ıµ¡´ù´û´ü´ş´÷"};
const unsigned char Pymb_dan[]   ={"µ¤µ¥µ£µ¢µ¦µ¨µ§µ©µ«µ®µ¯µ¬µ­µ°µª"};
const unsigned char Pymb_dang[]  ={"µ±µ²µ³µ´µµ"};
const unsigned char Pymb_dao[]   ={"µ¶µ¼µºµ¹µ·µ»µ¸µ½µ¿µÁµÀµ¾"};
const unsigned char Pymb_de[]    ={"µÃµÂµÄ"};
const unsigned char Pymb_deng[]  ={"µÆµÇµÅµÈµËµÊµÉ"};
const unsigned char Pymb_di[]    ={"µÍµÌµÎµÒµÏµĞµÓµÑµÕµ×µÖµØµÜµÛµİµÚµŞµÙ"};
const unsigned char Pymb_dian[]  ={"µàµáµßµäµãµâµçµèµéµêµæµëµíµìµîµå"};
const unsigned char Pymb_diao[]  ={"µóµğµòµïµñµõµöµô"};
const unsigned char Pymb_die[]   ={"µùµøµüµıµşµúµû"};
const unsigned char Pymb_ding[]  ={"¶¡¶£¶¢¶¤¶¥¶¦¶©¶¨¶§"};
const unsigned char Pymb_diu[]   ={"¶ª"};
const unsigned char Pymb_dong[]  ={"¶«¶¬¶­¶®¶¯¶³¶±¶²¶°¶´"};
const unsigned char Pymb_dou[]   ={"¶¼¶µ¶·¶¶¶¸¶¹¶º¶»"};
const unsigned char Pymb_du[]    ={"¶½¶¾¶Á¶¿¶À¶Â¶Ä¶Ã¶Ê¶Å¶Ç¶È¶É¶Æ"};
const unsigned char Pymb_duan[]  ={"¶Ë¶Ì¶Î¶Ï¶Ğ¶Í"};
const unsigned char Pymb_dui[]   ={"¶Ñ¶Ó¶Ô¶Ò"};
const unsigned char Pymb_dun[]   ={"¶Ö¶Ø¶Õ¶×¶Ü¶Û¶Ù¶İ"};
const unsigned char Pymb_duo[]   ={"¶à¶ß¶á¶Ş¶ä¶â¶ã¶ç¶é¶æ¶è¶å"};
const unsigned char Pymb_e[]     ={"¶ï¶í¶ğ¶ë¶ì¶ê¶î¶ò¶ó¶ñ¶ö¶õ¶ô"};
const unsigned char Pymb_en[]    ={"¶÷"};
const unsigned char Pymb_er[]    ={"¶ù¶ø¶û¶ú¶ı¶ü¶ş·¡"};
const unsigned char Pymb_f[]     ={"·É·Ö·¢·ç·¨·½"};
const unsigned char Pymb_fa[]    ={"·¢·¦·¥·£·§·¤·¨·©"};
const unsigned char Pymb_fan[]   ={"·«·¬·­·ª·²·¯·°·³·®·±·´·µ·¸·º·¹·¶··"};
const unsigned char Pymb_fang[]  ={"·½·»·¼·À·Á·¿·¾·Â·Ã·Ä·Å"};
const unsigned char Pymb_fei[]   ={"·É·Ç·È·Æ·Ê·Ë·Ì·Í·Ï·Ğ·Î·Ñ"};
const unsigned char Pymb_fen[]   ={"·Ö·Ô·×·Ò·Õ·Ó·Ø·Ú·Ù·Û·İ·Ü·Ş·ß·à"};
const unsigned char Pymb_feng[]  ={"·á·ç·ã·â·è·å·é·æ·ä·ë·ê·ì·í·ï·î"};
const unsigned char Pymb_fo[]    ={"·ğ"};
const unsigned char Pymb_fou[]   ={"·ñó¾"};
const unsigned char Pymb_fu[]    ={"·ò·ô¸¶¸§¸¸¸º¸®¸³¸©¸°¸»¸±¸£·ù·ø¸´¸¹·û¸¾¸«·õ¸¥·ü·ö·÷·ş·ı·ú¸¡¸¢¸¤¸À¸¦¸ª¸¨¸­¸¯¸¼·ó¸½¸·¸µ¸¿¸²"};
const unsigned char Pymb_g[]     ={"¸ö¸ù¸÷¸ß¹ı¸ø"};
const unsigned char Pymb_ga[]    ={"¸Â¸Á"};
const unsigned char Pymb_gai[]   ={"¸Ã¸Ä¸Æ¸Ç¸È¸Å"};
const unsigned char Pymb_gan[]   ={"¸É¸Ê¸Ë¸Î¸Í¸Ñ¸Ï¸Ò¸Ğ¸Ì¸Ó"};
const unsigned char Pymb_gang[]  ={"¸Ô¸Õ¸Ú¸Ù¸Ø¸×¸Ö¸Û¸Ü"};
const unsigned char Pymb_gao[]   ={"¸á¸ß¸æ¸ã¸à¸İ¸Ş¸â¸å¸ä"};
const unsigned char Pymb_ge[]    ={"¸ö¸÷¸ç¸ï¸è¸ñ¸î¸é¸ê¸í¸ì¸ë¸ó¸ğ¸ô¸õ¿©"};
const unsigned char Pymb_gei[]   ={"¸ø"};
const unsigned char Pymb_gen[]   ={"¸ù¸ú"};
const unsigned char Pymb_geng[]  ={"¸ü¸ı¸û¸ş¹¡¹¢¹£"};
const unsigned char Pymb_gong[]  ={"¹¤¹­¹«¹²¹¦¹¥¹©¹¬¹§¹ª¹¨¹®¹¯¹°¹±"};
const unsigned char Pymb_gou[]   ={"¹´¹µ¹³¹·¹¶¹¹¹º¹¸¹»"};
const unsigned char Pymb_gu[]    ={"¹À¹¾¹Ã¹Â¹Á¹½¹¼¹¿¹Å¹È¹É¹Ç¹Æ¹Ä¹Ì¹Ê¹Ë¹Í"};
const unsigned char Pymb_gua[]   ={"¹Ï¹Î¹Ğ¹Ñ¹Ò¹Ó"};
const unsigned char Pymb_guai[]  ={"¹Ô¹Õ¹Ö"};
const unsigned char Pymb_guan[]  ={"¹Ø¹Û¹Ù¹Ú¹×¹İ¹Ü¹á¹ß¹à¹Ş"};
const unsigned char Pymb_guang[] ={"¹â¹ã¹ä"};
const unsigned char Pymb_gui[]   ={"¹é¹ñ¹ğ¹í¹ì¹ò¹ó¹ç¹ê¹æ¹ë¹è¹å¹î¹ï¹ô"};
const unsigned char Pymb_gun[]   ={"¹õ¹ö¹÷ÙòçµíŞ"};
const unsigned char Pymb_guo[]   ={"¹ù¹ø¹ú¹û¹ü¹ıàşòå"};
const unsigned char Pymb_h[]     ={"ºÃºÍºó»¹»áºÜ"};
const unsigned char Pymb_ha[]    ={"¸ò¹ş"};
const unsigned char Pymb_hai[]   ={"»¹º¢º¡º£º¥º§º¦º¤àË"};
const unsigned char Pymb_han[]   ={"º¨º©º¬ºªº¯º­º®º«º±º°ººº¹ºµº·º´º¸º¶º³º²"};
const unsigned char Pymb_hang[]  ={"º¼º½ĞĞ"};
const unsigned char Pymb_hao[]   ={"ºÁºÀº¿º¾ºÃºÂºÅºÆºÄ"};
const unsigned char Pymb_he[]    ={"ºÇºÈºÌºÏºÎºÍºÓºÒºËºÉºÔºĞºÊºØºÖºÕº×"};
const unsigned char Pymb_hei[]   ={"ºÚºÙ"};
const unsigned char Pymb_hen[]   ={"ºÛºÜºİºŞ"};
const unsigned char Pymb_heng[]  ={"ºàºßºãºáºâ"};
const unsigned char Pymb_hong[]  ={"ºäºåºæºëºìºêºéºçºè"};
const unsigned char Pymb_hou[]   ={"ºîºíºïºğºóºñºò"};
const unsigned char Pymb_hu[]    ={"ºõºôºö»¡ºüºúºøºşºùº÷ºıºû»¢»£»¥»§»¤»¦"};
const unsigned char Pymb_hua[]   ={"»¨»ª»©»¬»«»¯»®»­»°"};
const unsigned char Pymb_huai[]  ={"»³»²»´»±»µ"};
const unsigned char Pymb_huan[]  ={"»¶»¹»·»¸»º»Ã»Â»½»»»Á»¼»À»¾»¿"};
const unsigned char Pymb_huang[] ={"»Ä»Å»Ê»Ë»Æ»Ì»Í»È»Ç»É»Ğ»Î»Ñ»Ï"};
const unsigned char Pymb_hui[]   ={"»Ò»Ö»Ó»Ô»Õ»Ø»×»Ú»Ü»ã»á»ä»æ»å»â»ß»Ş»à»İ»Ù»Û"};
const unsigned char Pymb_hun[]   ={"»è»ç»é»ë»ê»ì"};
const unsigned char Pymb_huo[]   ={"»í»î»ğ»ï»ò»õ»ñ»ö»ó»ô"};
const unsigned char Pymb_j[]     ={"¾Í½ø½«¼¸¾Å¼Ó"};
const unsigned char Pymb_ji[]    ={"¼¥»÷¼¢»ø»ú¼¡¼¦¼£¼§»ı»ù¼¨¼©»û»ş»ü¼¤¼°¼ª¼³¼¶¼´¼«¼±¼²¼¬¼¯¼µ¼­¼®¼¸¼º¼·¼¹¼Æ¼Ç¼¿¼Í¼Ë¼É¼¼¼Ê¼Á¼¾¼È¼Ã¼Ì¼Å¼Ä¼Â¼À¼»¼½½å"};
const unsigned char Pymb_jia[]   ={"¼Ó¼Ğ¼Ñ¼Ï¼Ò¼Î¼Ô¼Õ¼×¼Ö¼Ø¼Û¼İ¼Ü¼Ù¼Ş¼ÚĞ®"};
const unsigned char Pymb_jian[]  ={"¼é¼â¼á¼ß¼ä¼ç¼è¼æ¼à¼ã¼ê¼å¼ğ¼ó¼í¼ë¼ñ¼õ¼ô¼ì¼ï¼ò¼î¼û¼ş½¨½¤½£¼ö¼ú½¡½§½¢½¥½¦¼ù¼ø¼ü¼ı"};
const unsigned char Pymb_jiang[] ={"½­½ª½«½¬½©½®½²½±½°½¯½³½µ½´"};
const unsigned char Pymb_jiao[]  ={"½»½¼½¿½½½¾½º½·½¹½¶½¸½Ç½Æ½Ê½È½Ã½Å½Â½Á½Ë½É½Ğ½Î½Ï½Ì½Ñ½Í¾õ½À"};
const unsigned char Pymb_jie[]   ={"½×½Ô½Ó½Õ½Ò½Ö½Ú½Ù½Ü½à½á½İ½Ş½Ø½ß½ã½â½é½ä½æ½ì½ç½ê½ë½è"};
const unsigned char Pymb_jin[]   ={"½í½ñ½ï½ğ½ò½î½ó½ö½ô½÷½õ¾¡¾¢½ü½ø½ú½ş½ı½û½ù"};
const unsigned char Pymb_jing[]  ={"¾©¾­¾¥¾£¾ª¾§¾¦¾¬¾¤¾«¾¨¾®¾±¾°¾¯¾»¾¶¾·¾º¾¹¾´¾¸¾³¾²¾µ"};
const unsigned char Pymb_jiong[] ={"¾¼¾½"};
const unsigned char Pymb_jiu[]   ={"¾À¾¿¾¾¾Å¾Ã¾Ä¾Á¾Â¾Æ¾É¾Ê¾Ì¾Î¾Ç¾È¾Í¾Ë"};
const unsigned char Pymb_ju[]    ={"¾Ó¾Ğ¾Ñ¾Ô¾Ò¾Ï¾Ö½Û¾Õ¾×¾Ú¾Ù¾Ø¾ä¾Ş¾Ü¾ß¾æ¾ã¾ç¾å¾İ¾à¾â¾Û¾á"};
const unsigned char Pymb_juan[]  ={"¾ê¾è¾é¾í¾ë¾î¾ì"};
const unsigned char Pymb_jue[]   ={"¾ï¾ö¾÷¾ñ¾ø¾ó¾ò¾ô¾ğ"};
const unsigned char Pymb_jun[]   ={"¾ü¾ı¾ù¾û¾ú¿¡¿¤¾ş¿£¿¥¿¢"};
const unsigned char Pymb_k[]     ={"¿´¿ª¿É¿ì¿Ë¿×"};
const unsigned char Pymb_ka[]    ={"¿¨¿§¿¦"};
const unsigned char Pymb_kai[]   ={"¿ª¿«¿­¿®¿¬"};
const unsigned char Pymb_kan[]   ={"¼÷¿¯¿±¿°¿²¿³¿´"};
const unsigned char Pymb_kang[]  ={"¿µ¿¶¿·¿¸¿º¿¹¿»"};
const unsigned char Pymb_kao[]   ={"¿¼¿½¿¾¿¿"};
const unsigned char Pymb_ke[]    ={"¿À¿Á¿Â¿Æ¿Ã¿Å¿Ä¿Ç¿È¿É¿Ê¿Ë¿Ì¿Í¿Î"};
const unsigned char Pymb_ken[]   ={"¿Ï¿Ñ¿Ò¿Ğ"};
const unsigned char Pymb_keng[]  ={"¿Ô¿Ó"};
const unsigned char Pymb_kong[]  ={"¿Õ¿×¿Ö¿Ø"};
const unsigned char Pymb_kou[]   ={"¿Ù¿Ú¿Û¿Ü"};
const unsigned char Pymb_ku[]    ={"¿İ¿Ş¿ß¿à¿â¿ã¿á"};
const unsigned char Pymb_kua[]   ={"¿ä¿å¿æ¿è¿ç"};
const unsigned char Pymb_kuai[]  ={"¿é¿ì¿ë¿ê"};
const unsigned char Pymb_kuan[]  ={"¿í¿î"};
const unsigned char Pymb_kuang[] ={"¿ï¿ğ¿ñ¿ö¿õ¿ó¿ò¿ô"};
const unsigned char Pymb_kui[]   ={"¿÷¿ù¿ø¿ú¿ü¿û¿ı¿şÀ¢À£À¡"};
const unsigned char Pymb_kun[]   ={"À¤À¥À¦À§"};
const unsigned char Pymb_kuo[]   ={"À©À¨À«Àª"};
const unsigned char Pymb_l[]     ={"ÁËÁ½À´ÀïÀÏÂ¥"};
const unsigned char Pymb_la[]    ={"À¬À­À²À®À°À¯À±"};
const unsigned char Pymb_lai[]   ={"À´À³Àµ"};
const unsigned char Pymb_lan[]   ={"À¼À¹À¸À·À»À¶À¾À½ÀºÀÀÀ¿ÀÂÀÁÀÃÀÄ"};
const unsigned char Pymb_lang[]  ={"ÀÉÀÇÀÈÀÅÀÆÀÊÀË"};
const unsigned char Pymb_lao[]   ={"ÀÌÀÍÀÎÀÏÀĞÀÑÀÔÀÓÀÒ"};
const unsigned char Pymb_le[]    ={"ÀÖÀÕÁË"};
const unsigned char Pymb_lei[]   ={"À×ÀØÀİÀÚÀÙÀÜÀßÀáÀàÀÛÀŞ"};
const unsigned char Pymb_leng[]  ={"ÀâÀãÀä"};
const unsigned char Pymb_li[]    ={"ÀåÀæÀêÀëÀòÀçÀìÁ§ÀèÀéÀñÀîÀïÁ¨ÀíÀğÁ¦ÀúÀ÷Á¢ÀôÀöÀûÀøÁ¤ÀıÁ¥ÀşÀóÀõÀùÁ£ÀüÁ¡"};
const unsigned char Pymb_lian[]  ={"Á¬Á±Á¯Á°Á«ÁªÁ®Á­Á²Á³Á·Á¶ÁµÁ´"};
const unsigned char Pymb_liang[] ={"Á©Á¼Á¹ÁºÁ¸Á»Á½ÁÁÁÂÁ¾ÁÀÁ¿"};
const unsigned char Pymb_liao[]  ={"ÁÊÁÉÁÆÁÄÁÅÁÈÁÎÁÃÁÇÁÍÁÏÁÌ"};
const unsigned char Pymb_lie[]   ={"ÁĞÁÓÁÒÁÔÁÑ"};
const unsigned char Pymb_lin[]   ={"ÁÚÁÖÁÙÁÜÁÕÁØÁ×ÁÛÁİÁßÁŞÁà"};
const unsigned char Pymb_ling[]  ={"ÁæÁéÁëÁáÁèÁåÁêÁçÁâÁãÁäÁìÁîÁí"};
const unsigned char Pymb_liu[]   ={"ÁïÁõÁ÷ÁôÁğÁòÁóÁñÁöÁøÁù"};
const unsigned char Pymb_long[]  ={"ÁúÁüÁıÁûÂ¡ÁşÂ¤Â¢Â£"};
const unsigned char Pymb_lou[]   ={"Â¦Â¥Â§Â¨ÂªÂ©"};
const unsigned char Pymb_lu[]    ={"Â¶Â¬Â®Â«Â¯Â­Â±Â²Â°Â³Â½Â¼Â¸Â¹Â»ÂµÂ·Â¾ÂºÂ´"};
const unsigned char Pymb_luan[]  ={"ÂÏÂÍÂÎÂĞÂÑÂÒ"};
const unsigned char Pymb_lue[]   ={"ÂÓÂÔ"};
const unsigned char Pymb_lun[]   ={"ÂÕÂØÂ×ÂÙÂÚÂÖÂÛ"};
const unsigned char Pymb_luo[]   ={"ÂŞÂÜÂßÂàÂáÂâÂİÂãÂåÂçÂæÂä"};
const unsigned char Pymb_lv[]    ={"ÂËÂ¿ÂÀÂÂÂÃÂÁÂÅÂÆÂÄÂÉÂÇÂÊÂÌÂÈ"};
const unsigned char Pymb_m[]     ={"ÂğÃÇÃ»ÂíÃÅÃæ"};
const unsigned char Pymb_ma[]    ={"ÂèÂéÂíÂêÂëÂìÂîÂğÂï"};
const unsigned char Pymb_mai[]   ={"ÂñÂòÂõÂóÂôÂö"};
const unsigned char Pymb_man[]   ={"ÂùÂøÂ÷ÂúÂüÃ¡ÂıÂşÂû"};
const unsigned char Pymb_mang[]  ={"Ã¦Ã¢Ã¤Ã£Ã§Ã¥"};
const unsigned char Pymb_mao[]   ={"Ã¨Ã«Ã¬Ã©ÃªÃ®Ã­Ã¯Ã°Ã³Ã±Ã²"};
const unsigned char Pymb_me[]    ={"Ã´"};
const unsigned char Pymb_mei[]   ={"Ã»Ã¶ÃµÃ¼Ã·Ã½ÃºÃ¸Ã¹Ã¿ÃÀÃ¾ÃÃÃÁÃÄÃÂ"};
const unsigned char Pymb_men[]   ={"ÃÅÃÆÃÇ"};
const unsigned char Pymb_meng[]  ={"ÃÈÃËÃÊÃÍÃÉÃÌÃÏÃÎ"};
const unsigned char Pymb_mi[]    ={"ÃÖÃÔÃÕÃÑÃÓÃÒÃ×ÃĞÃÚÃÙÃØÃÜÃİÃÛ"};
const unsigned char Pymb_mian[]  ={"ÃßÃàÃŞÃâÃãÃäÃáÃåÃæ"};
const unsigned char Pymb_miao[]  ={"ÃçÃèÃéÃëÃìÃêÃîÃí"};
const unsigned char Pymb_mie[]   ={"ÃğÃï"};
const unsigned char Pymb_min[]   ={"ÃñÃóÃòÃöÃõÃô"};
const unsigned char Pymb_ming[]  ={"ÃûÃ÷ÃùÃúÃøÃü"};
const unsigned char Pymb_miu[]   ={"Ãı"};
const unsigned char Pymb_mo[]    ={"ºÑÃşÄ¡Ä£Ä¤Ä¦Ä¥Ä¢Ä§Ä¨Ä©Ä­Ä°ÄªÄ¯Ä®Ä«Ä¬"};
const unsigned char Pymb_mou[]   ={"Ä²Ä±Ä³"};
const unsigned char Pymb_mu[]    ={"Ä¸Ä¶ÄµÄ·Ä´Ä¾Ä¿ÄÁÄ¼Ä¹Ä»ÄÀÄ½ÄºÄÂ"};
const unsigned char Pymb_n[]     ={"ÄãÄÇÄêÄÚÄÜÄĞ"};
const unsigned char Pymb_na[]    ={"ÄÃÄÄÄÇÄÉÄÈÄÆÄÅ"};
const unsigned char Pymb_nai[]   ={"ÄËÄÌÄÊÄÎÄÍ"};
const unsigned char Pymb_nan[]   ={"ÄĞÄÏÄÑ"};
const unsigned char Pymb_nang[]  ={"ÄÒ"};
const unsigned char Pymb_nao[]   ={"ÄÓÄÕÄÔÄÖÄ×"};
const unsigned char Pymb_ne[]    ={"ÄØ"};
const unsigned char Pymb_nei[]   ={"ÄÚÄÙ"};
const unsigned char Pymb_nen[]   ={"ÄÛ"};
const unsigned char Pymb_neng[]  ={"ÄÜ"};
const unsigned char Pymb_ni[]    ={"ÄİÄáÄàÄßÄŞÄãÄâÄæÄäÄçÄå"};
const unsigned char Pymb_nian[]  ={"ÄéÄêÄíÄìÄëÄîÄè"};
const unsigned char Pymb_niang[] ={"ÄïÄğ"};
const unsigned char Pymb_niao[]  ={"ÄñÄò"};
const unsigned char Pymb_nie[]   ={"ÄóÄùÄôÄöÄ÷ÄøÄõ"};
const unsigned char Pymb_nin[]   ={"Äú"};
const unsigned char Pymb_ning[]  ={"ÄşÅ¡ÄüÄûÄıÅ¢"};
const unsigned char Pymb_niu[]   ={"Å£Å¤Å¦Å¥"};
const unsigned char Pymb_nong[]  ={"Å©Å¨Å§Åª"};
const unsigned char Pymb_nu[]    ={"Å«Å¬Å­"};
const unsigned char Pymb_nuan[]  ={"Å¯"};
const unsigned char Pymb_nue[]   ={"Å±Å°"};
const unsigned char Pymb_nuo[]   ={"Å²ÅµÅ³Å´"};
const unsigned char Pymb_nv[]    ={"Å®"};
const unsigned char Pymb_o[]     ={"Å¶àŞ"};
const unsigned char Pymb_ou[]    ={"Å·Å¹Å¸Å»Å¼ÅºÅ½"};
const unsigned char Pymb_p[]     ={"Æ½Æ¬Æ·ÆÀÆ±ÅÂ"};
const unsigned char Pymb_pa[]    ={"Å¿Å¾ÅÀ°ÒÅÃÅÁÅÂ"};
const unsigned char Pymb_pai[]   ={"ÅÄÅÇÅÅÅÆÅÉÅÈ"};
const unsigned char Pymb_pan[]   ={"ÅËÅÊÅÌÅÍÅĞÅÑÅÎÅÏ"};
const unsigned char Pymb_pang[]  ={"ÅÒÅÓÅÔÅÕÅÖ"};
const unsigned char Pymb_pao[]   ={"Å×ÅÙÅØÅÚÅÛÅÜÅİ"};
const unsigned char Pymb_pei[]   ={"ÅŞÅßÅãÅàÅâÅáÅæÅåÅä"};
const unsigned char Pymb_pen[]   ={"ÅçÅè"};
const unsigned char Pymb_peng[]  ={"ÅêÅéÅëÅóÅíÅïÅğÅîÅôÅìÅñÅòÅõÅö"};
const unsigned char Pymb_pi[]    ={"±ÙÅúÅ÷ÅûÅøÅüÅùÆ¤ÅşÆ£Æ¡ÅıÆ¢Æ¥Æ¦Æ¨Æ§Æ©"};
const unsigned char Pymb_pian[]  ={"Æ¬Æ«ÆªÆ­"};
const unsigned char Pymb_piao[]  ={"Æ¯Æ®Æ°Æ±"};
const unsigned char Pymb_pie[]   ={"Æ²Æ³"};
const unsigned char Pymb_pin[]   ={"Æ´Æ¶ÆµÆ·Æ¸"};
const unsigned char Pymb_ping[]  ={"Æ¹Æ½ÆÀÆ¾ÆºÆ»ÆÁÆ¿Æ¼"};
const unsigned char Pymb_po[]    ={"ÆÂÆÃÆÄÆÅÆÈÆÆÆÉÆÇ"};
const unsigned char Pymb_pou[]   ={"ÆÊ"};
const unsigned char Pymb_pu[]    ={"¸¬ÆÍÆËÆÌÆÎÆĞÆÏÆÑÆÓÆÔÆÒÆÖÆÕÆ×ÆØ"};
const unsigned char Pymb_q[]     ={"ÆßÈ¥ÆäÈ«ÆÚÇ°"};
const unsigned char Pymb_qi[]    ={"ÆßÆãÆŞÆâÆàÆÜÆİÆÚÆÛÆáÆîÆëÆäÆæÆçÆíÆêÆéÆèÆïÆåÆìÆòÆóÆñÆôÆğÆøÆıÆùÆúÆûÆüÆõÆöÆ÷"};
const unsigned char Pymb_qia[]   ={"ÆşÇ¡Ç¢"};
const unsigned char Pymb_qian[]  ={"Ç§ÇªÇ¤Ç¨Ç¥Ç£Ç¦Ç«Ç©Ç°Ç®Ç¯Ç¬Ç±Ç­Ç³Ç²Ç´Ç·ÇµÇ¶Ç¸"};
const unsigned char Pymb_qiang[] ={"ÇºÇ¼Ç¹Ç»Ç¿Ç½Ç¾ÇÀ"};
const unsigned char Pymb_qiao[]  ={"ÇÄÇÃÇÂÇÁÇÇÇÈÇÅÇÆÇÉÇÎÇÍÇÏÇÌÇËÇÊ"};
const unsigned char Pymb_qie[]   ={"ÇĞÇÑÇÒÇÓÇÔ"};
const unsigned char Pymb_qin[]   ={"Ç×ÇÖÇÕÇÛÇØÇÙÇİÇÚÇÜÇŞÇß"};
const unsigned char Pymb_qing[]  ={"ÇàÇâÇáÇãÇäÇåÇéÇçÇèÇæÇêÇëÇì"};
const unsigned char Pymb_qiong[] ={"ÇîÇí"};
const unsigned char Pymb_qiu[]   ={"ÇğÇñÇïÇôÇóÇöÇõÇò"};
const unsigned char Pymb_qu[]    ={"ÇøÇúÇıÇüÇùÇûÇ÷ÇşÈ¡È¢È£È¥È¤"};
const unsigned char Pymb_quan[]  ={"È¦È«È¨ÈªÈ­È¬È©È§È®È°È¯"};
const unsigned char Pymb_que[]   ={"È²È±È³È´È¸È·ÈµÈ¶"};
const unsigned char Pymb_qun[]   ={"È¹Èº"};
const unsigned char Pymb_r[]     ={"ÈËÈçÈÕÈÃÈôÈë"};
const unsigned char Pymb_ran[]   ={"È»È¼È½È¾"};
const unsigned char Pymb_rang[]  ={"È¿ÈÂÈÀÈÁÈÃ"};
const unsigned char Pymb_rao[]   ={"ÈÄÈÅÈÆ"};
const unsigned char Pymb_re[]    ={"ÈÇÈÈ"};
const unsigned char Pymb_ren[]   ={"ÈËÈÊÈÉÈÌÈĞÈÏÈÎÈÒÈÑÈÍ"};
const unsigned char Pymb_reng[]  ={"ÈÓÈÔ"};
const unsigned char Pymb_ri[]    ={"ÈÕ"};
const unsigned char Pymb_rong[]  ={"ÈÖÈŞÈ×ÈÙÈİÈÜÈØÈÛÈÚÈß"};
const unsigned char Pymb_rou[]   ={"ÈáÈàÈâ"};
const unsigned char Pymb_ru[]    ={"ÈçÈãÈåÈæÈäÈêÈéÈèÈëÈì"};
const unsigned char Pymb_ruan[]  ={"ÈîÈí"};
const unsigned char Pymb_rui[]   ={"ÈïÈñÈğ"};
const unsigned char Pymb_run[]   ={"ÈòÈó"};
const unsigned char Pymb_ruo[]   ={"ÈôÈõ"};
const unsigned char Pymb_s[]     ={"ÊÇËÄÉÏËµÊÂÊ±"};
const unsigned char Pymb_sa[]    ={"ÈöÈ÷Èø"};
const unsigned char Pymb_sai[]   ={"ÈûÈùÈúÈü"};
const unsigned char Pymb_san[]   ={"ÈıÈşÉ¡É¢"};
const unsigned char Pymb_sang[]  ={"É£É¤É¥"};
const unsigned char Pymb_sao[]   ={"É¦É§É¨É©"};
const unsigned char Pymb_se[]    ={"É«É¬Éª"};
const unsigned char Pymb_sen[]   ={"É­"};
const unsigned char Pymb_seng[]  ={"É®"};
const unsigned char Pymb_sha[]   ={"É±É³É´É°É¯ÉµÉ¶É·ÏÃ"};
const unsigned char Pymb_shai[]  ={"É¸É¹"};
const unsigned char Pymb_shan[]  ={"É½É¾É¼ÉÀÉºÉ¿ÉÁÉÂÉÇÉ»ÉÈÉÆÉÉÉÃÉÅÉÄÕ¤"};
const unsigned char Pymb_shang[] ={"ÉËÉÌÉÊÉÑÉÎÉÍÉÏÉĞ"};
const unsigned char Pymb_shao[]  ={"ÉÓÉÒÉÕÉÔÉ×ÉÖÉØÉÙÉÛÉÜÉÚ"};
const unsigned char Pymb_she[]   ={"ÉİÉŞÉàÉßÉáÉèÉçÉäÉæÉâÉåÉã"};
const unsigned char Pymb_shen[]  ={"ÉêÉìÉíÉëÉğÉïÉéÉîÉñÉòÉóÉôÉöÉõÉøÉ÷Ê²"};
const unsigned char Pymb_sheng[] ={"ÉıÉúÉùÉüÊ¤ÉûÉşÊ¡Ê¥Ê¢Ê£"};
const unsigned char Pymb_shi[]   ={"³×Ê¬Ê§Ê¦Ê­Ê«Ê©Ê¨ÊªÊ®Ê¯Ê±Ê¶ÊµÊ°Ê´Ê³Ê·Ê¸Ê¹Ê¼Ê»ÊºÊ¿ÊÏÊÀÊËÊĞÊ¾Ê½ÊÂÊÌÊÆÊÓÊÔÊÎÊÒÊÑÊÃÊÇÊÁÊÊÊÅÊÍÊÈÊÄÊÉËÆ"};
const unsigned char Pymb_shou[]  ={"ÊÕÊÖÊØÊ×ÊÙÊÜÊŞÊÛÊÚÊİ"};
const unsigned char Pymb_shu[]   ={"ÊéÊãÊåÊàÊâÊáÊçÊèÊæÊäÊßÊëÊêÊìÊîÊòÊğÊóÊñÊíÊïÊõÊùÊøÊöÊ÷ÊúË¡ÊüÊıÊûÊşÊô"};
const unsigned char Pymb_shua[]  ={"Ë¢Ë£"};
const unsigned char Pymb_shuai[] ={"Ë¥Ë¤Ë¦Ë§"};
const unsigned char Pymb_shuan[] ={"Ë©Ë¨"};
const unsigned char Pymb_shuang[]={"Ë«ËªË¬"};
const unsigned char Pymb_shui[]  ={"Ë­Ë®Ë°Ë¯"};
const unsigned char Pymb_shun[]  ={"Ë±Ë³Ë´Ë²"};
const unsigned char Pymb_shuo[]  ={"ËµË¸Ë·Ë¶"};
const unsigned char Pymb_si[]    ={"Ë¿Ë¾Ë½Ë¼Ë¹Ë»ËºËÀËÈËÄËÂËÅËÇËÃËÁ"};
const unsigned char Pymb_song[]  ={"ËÉËËËÊËÏËÎËĞËÍËÌ"};
const unsigned char Pymb_sou[]   ={"ËÔËÑËÒËÓ"};
const unsigned char Pymb_su[]    ={"ËÕËÖË×ËßËàËØËÙËÚËÜËİËÛ"};
const unsigned char Pymb_suan[]  ={"ËáËâËã"};
const unsigned char Pymb_sui[]   ={"ËäËçËåËæËèËêËîËìËéËíËë"};
const unsigned char Pymb_sun[]   ={"ËïËğËñ"};
const unsigned char Pymb_suo[]   ={"ËôËóËòËõËùË÷ËöËø"};
const unsigned char Pymb_t[]     ={"ËûËıÌ«Í·ÌìÍ¬"};
const unsigned char Pymb_ta[]    ={"ËıËûËüËúËşÌ¡Ì¢Ì¤Ì£"};
const unsigned char Pymb_tai[]   ={"Ì¥Ì¨Ì§Ì¦Ì«Ì­Ì¬Ì©Ìª"};
const unsigned char Pymb_tan[]   ={"Ì®Ì°Ì¯Ì²Ì±Ì³Ì¸ÌµÌ·Ì¶Ì´Ì¹Ì»ÌºÌ¾Ì¿Ì½Ì¼"};
const unsigned char Pymb_tang[]  ={"ÌÀÌÆÌÃÌÄÌÁÌÂÌÅÌÇÌÈÌÊÌÉÌÌÌË"};
const unsigned char Pymb_tao[]   ={"ÌÎÌĞÌÍÌÏÌÓÌÒÌÕÌÔÌÑÌÖÌ×"};
const unsigned char Pymb_te[]    ={"ÌØ"};
const unsigned char Pymb_teng[]  ={"ÌÛÌÚÌÜÌÙ"};
const unsigned char Pymb_ti[]    ={"ÌŞÌİÌàÌßÌäÌáÌâÌãÌåÌëÌêÌéÌèÌæÌç"};
const unsigned char Pymb_tian[]  ={"ÌìÌíÌïÌñÌğÌîÌóÌò"};
const unsigned char Pymb_tiao[]  ={"µ÷ÌôÌõÌöÌ÷Ìø"};
const unsigned char Pymb_tie[]   ={"ÌùÌúÌû"};
const unsigned char Pymb_ting[]  ={"ÌüÍ¡ÌıÌşÍ¢Í¤Í¥Í£Í¦Í§"};
const unsigned char Pymb_tong[]  ={"Í¨Í¬Í®Í©Í­Í¯ÍªÍ«Í³Í±Í°Í²Í´"};
const unsigned char Pymb_tou[]   ={"ÍµÍ·Í¶Í¸"};
const unsigned char Pymb_tu[]    ={"Í¹ÍºÍ»Í¼Í½Í¿Í¾ÍÀÍÁÍÂÍÃ"};
const unsigned char Pymb_tuan[]  ={"ÍÄÍÅ"};
const unsigned char Pymb_tui[]   ={"ÍÆÍÇÍÈÍËÍÉÍÊ"};
const unsigned char Pymb_tun[]   ={"¶ÚÍÌÍÍÍÎ"};
const unsigned char Pymb_tuo[]   ={"ÍĞÍÏÍÑÍÔÍÓÍÕÍÒÍ×ÍÖÍØÍÙ"};
const unsigned char Pymb_w[]     ={"ÎÒÎªÎåÍòÎŞÍõ"};
const unsigned char Pymb_wa[]    ={"ÍÛÍŞÍÚÍİÍÜÍßÍà"};
const unsigned char Pymb_wai[]   ={"ÍáÍâ"};
const unsigned char Pymb_wan[]   ={"ÍäÍåÍãÍèÍêÍæÍçÍéÍğÍìÍíÍñÍïÍîÍëÍòÍó"};
const unsigned char Pymb_wang[]  ={"ÍôÍöÍõÍøÍùÍ÷ÍıÍüÍúÍû"};
const unsigned char Pymb_wei[]   ={"Î£ÍşÎ¢ÎºÎ¡ÎªÎ¤Î§Î¥Î¦Î¨Î©Î¬Î«Î°Î±Î²Î³Î­Î¯Î®ÎÀÎ´Î»Î¶Î·Î¸Î¾Î½Î¹Î¼ÎµÎ¿"};
const unsigned char Pymb_wen[]   ={"ÎÂÎÁÎÄÎÆÎÅÎÃÎÇÎÉÎÈÎÊ"};
const unsigned char Pymb_weng[]  ={"ÎÌÎËÎÍ"};
const unsigned char Pymb_wo[]    ={"ÎÎÎĞÎÑÎÏÎÒÎÖÎÔÎÕÎÓ"};
const unsigned char Pymb_wu[]    ={"ÎÚÎÛÎØÎ×ÎİÎÜÎÙÎŞÎãÎâÎáÎßÎàÎåÎçÎéÎëÎäÎêÎæÎèÎğÎñÎìÎïÎóÎòÎîÎí"};
const unsigned char Pymb_x[]     ={"ÏëĞ¡ÏÂÏÈÑ§ÏòÏÖĞÄÏñĞÍĞÂÏØ"};
const unsigned char Pymb_xi[]    ={"Ï¦Ï«Î÷ÎüÏ£ÎôÎöÎùÏ¢ÎşÏ¤Ï§Ï©ÎøÎúÏ¬Ï¡ÏªÎıÏ¨ÎõÎûÏ¥Ï°Ï¯Ï®Ï±Ï­Ï´Ï²Ï·ÏµÏ¸Ï¶"};
const unsigned char Pymb_xia[]   ={"ÏºÏ¹Ï»ÏÀÏ¿ÏÁÏ¾Ï½Ï¼ÏÂÏÅÏÄ"};
const unsigned char Pymb_xian[]  ={"Ï³ÏÉÏÈÏËÏÆÏÇÏÊÏĞÏÒÏÍÏÌÏÑÏÏÏÎÏÓÏÔÏÕÏØÏÖÏßÏŞÏÜÏİÏÚÏÛÏ×ÏÙ"};
const unsigned char Pymb_xiang[] ={"ÏçÏàÏãÏáÏæÏäÏåÏâÏêÏéÏèÏíÏìÏëÏòÏïÏîÏóÏñÏğ"};
const unsigned char Pymb_xiao[]  ={"ÏüÏûÏôÏõÏúÏöÏùÏıĞ¡ÏşĞ¢Ğ¤ÏøĞ§Ğ£Ğ¦Ğ¥"};
const unsigned char Pymb_xie[]   ={"Ğ©Ğ¨ĞªĞ«Ğ­Ğ°Ğ²Ğ±Ğ³Ğ¯Ğ¬Ğ´Ğ¹ĞºĞ¶Ğ¼ĞµĞ»Ğ¸Ğ·"};
const unsigned char Pymb_xin[]   ={"ĞÄĞÃĞ¾ĞÁĞÀĞ¿ĞÂĞ½ĞÅĞÆ"};
const unsigned char Pymb_xing[]  ={"ĞËĞÇĞÊĞÉĞÈĞÌĞÏĞÎĞÍĞÑĞÓĞÕĞÒĞÔ"};
const unsigned char Pymb_xiong[] ={"Ğ×ĞÖĞÙĞÚĞØĞÛĞÜ"};
const unsigned char Pymb_xiu[]   ={"ËŞĞİĞŞĞßĞàĞãĞåĞäĞâĞá"};
const unsigned char Pymb_xu[]    ={"ĞçĞëĞéĞêĞèĞæĞìĞíĞñĞòĞğĞôĞ÷ĞøĞïĞöĞõĞîÓõ"};
const unsigned char Pymb_xuan[]  ={"ĞùĞûĞúĞşĞüĞıÑ¡Ñ¢Ñ¤Ñ£"};
const unsigned char Pymb_xue[]   ={"Ï÷Ñ¥Ñ¦Ñ¨Ñ§Ñ©Ñª"};
const unsigned char Pymb_xun[]   ={"Ñ«Ñ¬Ñ°Ñ²Ñ®Ñ±Ñ¯Ñ­ÑµÑ¶Ñ´Ñ¸Ñ·Ñ³"};
const unsigned char Pymb_y[]     ={"Ò»ÔÂÒ²ÓĞÓÃÒª"};
const unsigned char Pymb_ya[]    ={"Ñ¾Ñ¹Ñ½ÑºÑ»Ñ¼ÑÀÑ¿ÑÁÑÂÑÄÑÃÑÆÑÅÑÇÑÈ"};
const unsigned char Pymb_yan[]   ={"ÑÊÑÌÑÍÑÉÑËÑÓÑÏÑÔÑÒÑØÑ×ÑĞÑÎÑÖÑÑÑÕÑÙÑÜÑÚÑÛÑİÑáÑåÑâÑäÑçÑŞÑéÑèÑßÑæÑãÑà"};
const unsigned char Pymb_yang[]  ={"ÑëÑêÑíÑìÑïÑòÑôÑîÑğÑñÑóÑöÑøÑõÑ÷ÑùÑú"};
const unsigned char Pymb_yao[]   ={"½ÄÑıÑüÑûÒ¢Ò¦Ò¤Ò¥Ò¡Ò£ÑşÒ§Ò¨Ò©ÒªÒ«Ô¿"};
const unsigned char Pymb_ye[]    ={"Ò¬Ò­Ò¯Ò®Ò²Ò±Ò°ÒµÒ¶Ò·Ò³Ò¹Ò´ÒºÒ¸"};
const unsigned char Pymb_yi[]    ={"Ò»ÒÁÒÂÒ½ÒÀÒ¿Ò¼Ò¾ÒÇÒÄÒÊÒËÒÌÒÈÒÆÒÅÒÃÒÉÒÍÒÒÒÑÒÔÒÓÒÏÒĞÒÎÒåÒÚÒäÒÕÒéÒàÒÙÒìÒÛÒÖÒëÒØÒ×ÒïÒèÒßÒæÒêÒîÒİÒâÒçÒŞÒáÒãÒíÒÜ"};
const unsigned char Pymb_yin[]   ={"ÒòÒõÒöÒğÒñÒôÒóÒ÷ÒúÒùÒøÒüÒıÒûÒşÓ¡ÒóØ·Ûó"};
const unsigned char Pymb_ying[]  ={"Ó¦Ó²Ó°ÓªÓ­Ó³Ó¬Ó®Ó¥Ó¢Ó±Ó¨Ó¯Ó¤Ó£Ó§Ó©Ó«İº"};
const unsigned char Pymb_yo[]    ={"Ó´"};
const unsigned char Pymb_yong[]  ={"Ó¶ÓµÓ¸Ó¹ÓºÓ·ÓÀÓ½Ó¾ÓÂÓ¿ÓÁÓ¼Ó»ÓÃ"};
const unsigned char Pymb_you[]   ={"ÓĞÓÖÓÉÓÒÓÍÓÎÓ×ÓÅÓÑÓÇÓÄÓÆÓÈÓÌÓÊÓËÓÏÓÓÓÕÓÔØü"};
const unsigned char Pymb_yu[]    ={"ÓëÓûÓØÓÙÓåÓèÓàÓÛÓãÓáÓéÓæÓçÓäÓâÓŞÓÜÓİÓßÓîÓìÓğÓêÓíÓïÓñÔ¦ÓóÓıÓôÓüÓøÔ¡Ô¤ÓòÓûÓ÷Ô¢ÓùÔ£ÓöÓúÓşÔ¥"};
const unsigned char Pymb_yuan[]  ={"Ô©Ô§Ô¨ÔªÔ±Ô°Ô«Ô­Ô²Ô¬Ô®ÔµÔ´Ô³Ô¯Ô¶Ô·Ô¹ÔºÔ¸"};
const unsigned char Pymb_yue[]   ={"Ô»Ô¼ÔÂÔÀÔÃÔÄÔ¾ÔÁÔ½"};
const unsigned char Pymb_yun[]   ={"ÔÆÔÈÔÇÔÅÔÊÔÉÔĞÔËÔÎÔÍÔÏÔÌ"};
const unsigned char Pymb_z[]     ={"ÔÚÕâÖĞÖ®Ö»×ö"};
const unsigned char Pymb_za[]    ={"ÔÑÔÓÔÒÕ¦"};
const unsigned char Pymb_zai[]   ={"ÔÖÔÕÔÔÔ×ÔØÔÙÔÚ×Ğ"};
const unsigned char Pymb_zan[]   ={"ÔÛÔÜÔİÔŞ"};
const unsigned char Pymb_zang[]  ={"ÔßÔàÔá"};
const unsigned char Pymb_zao[]   ={"ÔâÔãÔäÔçÔæÔéÔèÔåÔîÔíÔìÔëÔïÔê"};
const unsigned char Pymb_ze[]    ={"ÔòÔñÔóÔğ"};
const unsigned char Pymb_zei[]   ={"Ôô"};
const unsigned char Pymb_zen[]   ={"Ôõ"};
const unsigned char Pymb_zeng[]  ={"ÔöÔ÷Ôù"};
const unsigned char Pymb_zha[]   ={"ÔûÔüÔúÔıÔşÕ¢Õ¡Õ£Õ§Õ©Õ¨Õ¥×õ"};
const unsigned char Pymb_zhai[]  ={"Õ«ÕªÕ¬µÔÕ­Õ®Õ¯"};
const unsigned char Pymb_zhan[]  ={"Õ´Õ±Õ³Õ²Õ°Õ¶Õ¹ÕµÕ¸Õ·Õ¼Õ½Õ»Õ¾ÕÀÕ¿Õº"};
const unsigned char Pymb_zhang[] ={"³¤ÕÅÕÂÕÃÕÄÕÁÕÇÕÆÕÉÕÌÕÊÕÈÕÍÕËÕÏÕÎ"};
const unsigned char Pymb_zhao[]  ={"ÕĞÕÑÕÒÕÓÕÙÕ×ÕÔÕÕÕÖÕØ×¦"};
const unsigned char Pymb_zhe[]   ={"ÕÚÕÛÕÜÕİÕŞÕßÕàÕâÕãÕá×Å"};
const unsigned char Pymb_zhen[]  ={"ÕêÕëÕìÕäÕæÕèÕåÕçÕéÕïÕíÕîÕóÕñÕòÕğÖ¡"};
const unsigned char Pymb_zheng[] ={"ÕùÕ÷ÕúÕõÕøÕöÕôÕüÕûÕıÖ¤Ö£ÕşÖ¢"};
const unsigned char Pymb_zhi[]   ={"Ö®Ö§Ö­Ö¥Ö¨Ö¦ÖªÖ¯Ö«Ö¬Ö©Ö´Ö¶Ö±ÖµÖ°Ö²Ö³Ö¹Ö»Ö¼Ö·Ö½Ö¸ÖºÖÁÖ¾ÖÆÖÄÖÎÖËÖÊÖÅÖ¿ÖÈÖÂÖÀÖÌÖÏÖÇÖÍÖÉÖÃ"};
const unsigned char Pymb_zhong[] ={"ÖĞÖÒÖÕÖÑÖÓÖÔÖ×ÖÖÖÙÖÚÖØ"};
const unsigned char Pymb_zhou[]  ={"ÖİÖÛÖßÖÜÖŞÖàÖáÖâÖãÖäÖæÖçÖåÖè"};
const unsigned char Pymb_zhu[]   ={"ÖìÖïÖêÖéÖîÖíÖëÖñÖòÖğÖ÷ÖôÖóÖöÖõ×¡Öú×¢Öü×¤Öù×£ÖøÖûÖşÖı"};
const unsigned char Pymb_zhua[]  ={"×¥"};
const unsigned char Pymb_zhuai[] ={"×§"};
const unsigned char Pymb_zhuan[] ={"×¨×©×ª×«×­"};
const unsigned char Pymb_zhuang[]={"×±×¯×®×°×³×´´±×²"};
const unsigned char Pymb_zhui[]  ={"×·×µ×¶×¹×º×¸"};
const unsigned char Pymb_zhun[]  ={"×»×¼"};
const unsigned char Pymb_zhuo[]  ={"×¿×¾×½×À×Æ×Â×Ç×Ã×Ä×Á"};
const unsigned char Pymb_zi[]    ={"×Î×È×É×Ë×Ê×Í×Ì×Ñ×Ó×Ï×Ò×Ö×Ô×Õ"};
const unsigned char Pymb_zong[]  ={"×Ú×Û×Ø×Ù×××Ü×İ"};
const unsigned char Pymb_zou[]   ={"×Ş×ß×à×á"};
const unsigned char Pymb_zu[]    ={"×â×ã×ä×å×ç×è×é×æ"};
const unsigned char Pymb_zuan[]  ={"×¬×ë×ê"};
const unsigned char Pymb_zui[]   ={"×ì×î×ï×í"};
const unsigned char Pymb_zun[]   ={"×ğ×ñ"};
const unsigned char Pymb_zuo[]   ={"×ò×ó×ô×÷×ø×ù×ö"};
const unsigned char Pymb_space[] ={""};

/*"Æ´ÒôÊäÈë·¨²éÑ¯Âë±í,¶ş¼¶×ÖÄ¸Ë÷Òı±í(index)"*/
const struct _PyIndex PyIndex_a[]={{"a",Pymb_a},
                                    {"ai",Pymb_ai},
                                    {"an",Pymb_an},
                                    {"ang",Pymb_ang},
                                    {"ao",Pymb_ao},};
const struct _PyIndex PyIndex_b[]={{"b",Pymb_b},
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
const struct _PyIndex PyIndex_c[]={{"c",Pymb_c},
                                    {"ca",Pymb_ca},
                                    {"cai",Pymb_cai},
                                    {"can",Pymb_can},
                                    {"cang",Pymb_cang},
                                    {"cao",Pymb_cao},
                                    {"ce",Pymb_ce},
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
const struct _PyIndex PyIndex_d[]={{"d",Pymb_d},
                                    {"da",Pymb_da},
                                    {"dai",Pymb_dai},
                                    {"dan",Pymb_dan},
                                    {"dang",Pymb_dang},
                                    {"dao",Pymb_dao},
                                    {"de",Pymb_de},
                                    {"deng",Pymb_deng},
                                    {"di",Pymb_di},
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
const struct _PyIndex PyIndex_e[]={{"e",Pymb_e},
                                    {"en",Pymb_en},
                                    {"er",Pymb_er}};
const struct _PyIndex PyIndex_f[]={{"f",Pymb_f},
                                    {"fa",Pymb_fa},
                                    {"fan",Pymb_fan},
                                    {"fang",Pymb_fang},
                                    {"fei",Pymb_fei},
                                    {"fen",Pymb_fen},
                                    {"feng",Pymb_feng},
                                    {"fo",Pymb_fo},
                                    {"fou",Pymb_fou},
                                    {"fu",Pymb_fu}};
const struct _PyIndex PyIndex_g[]={{"g",Pymb_g},
                                    {"ga",Pymb_ga},
                                    {"gai",Pymb_gai},
                                    {"gan",Pymb_gan},
                                    {"gang",Pymb_gang},
                                    {"gao",Pymb_gao},
                                    {"ge",Pymb_ge},
                                    {"gei",Pymb_gei},
                                    {"gen",Pymb_gan},
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
const struct _PyIndex PyIndex_h[]={{"h",Pymb_h},
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
//const struct _PyIndex PyIndex_i[]={{"i",Pymb_space}};
const struct _PyIndex PyIndex_j[]={{"j",Pymb_j},
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
const struct _PyIndex PyIndex_k[]={{"k",Pymb_da},
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
const struct _PyIndex PyIndex_l[]={{"l",Pymb_l},
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
                                    {"long",Pymb_long},
                                    {"lou",Pymb_lou},
                                    {"lu",Pymb_lu},
                                    {"luan",Pymb_luan},
                                    {"lue",Pymb_lue},
                                    {"lun",Pymb_lun},
                                    {"luo",Pymb_luo},
                                    {"lv",Pymb_lv}};
const struct _PyIndex PyIndex_m[]={{"m",Pymb_m},
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
const struct _PyIndex PyIndex_n[]={{"n",Pymb_n},
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
                                    {"nong",Pymb_nong},
                                    {"nu",Pymb_nu},
                                    {"nuan",Pymb_nuan},
                                    {"nue",Pymb_nue},
                                    {"nuo",Pymb_nuo},
                                    {"nv",Pymb_nv}};
const struct _PyIndex PyIndex_o[]={{"o",Pymb_o},
                                    {"ou",Pymb_ou}};
const struct _PyIndex PyIndex_p[]={{"p",Pymb_p},
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
const struct _PyIndex PyIndex_q[]={{"q",Pymb_q},
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
const struct _PyIndex PyIndex_r[]={{"r",Pymb_r},
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
const struct _PyIndex PyIndex_s[]={{"s",Pymb_s},
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
const struct _PyIndex PyIndex_t[]={{"t",Pymb_t},
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
//const struct _PyIndex PyIndex_u[]={{"u",Pymb_space}};
//const struct _PyIndex PyIndex_v[]={{"v",Pymb_space}};
const struct _PyIndex PyIndex_w[]={{"w",Pymb_w},
                                    {"wa",Pymb_wa},
                                    {"wai",Pymb_wai},
                                    {"wan",Pymb_wan},
                                    {"wang",Pymb_wang},
                                    {"wei",Pymb_wei},
                                    {"wen",Pymb_wen},
                                    {"weng",Pymb_weng},
                                    {"wo",Pymb_wo},
                                    {"wu",Pymb_wu}};
const struct _PyIndex PyIndex_x[]={{"x",Pymb_x},
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
const struct _PyIndex PyIndex_y[]={ {"y",Pymb_y},
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
const struct _PyIndex PyIndex_z[]={{"z",Pymb_z},
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
const struct _PyIndex **NumKeyPyIme_pGetCur(const PinYinIME_t *pPy)
{
  return (const struct _PyIndex **)(&(pPy->pPyTblIndex[pPy->CurPos]));
}

//------------------------------µÃµ½µ±Ç°×ÖÄ¸±í×ÜÊı--------------------------------
unsigned char NumKeyPyIme_GetCurCount(const PinYinIME_t *pPy)
{
  return pPy->PerFindCount[pPy->NumCount - 1];
}
                                                      


