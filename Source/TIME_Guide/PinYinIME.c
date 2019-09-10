/*
           pyInput.c - ƴ�����뷨ģ��

           ԭ��:�� ��
           ��д:�� ǿ(mail2li@21cn.com)
           �ظ�д��dap_ccj
*/

#include <string.h>
#include <stdio.h>

#include "PinYinIME.h"

/* "ƴ�����뷨�������б�,���(mb)" */
const unsigned char Pymb_a[]     ={"������"};
const unsigned char Pymb_ai[]    ={"����������������������������"};
const unsigned char Pymb_an[]    ={"��������������������"};
const unsigned char Pymb_ang[]   ={"������"};
const unsigned char Pymb_ao[]    ={"�������°��ð�����������"};
const unsigned char Pymb_b[]     ={"�ɰ˲����ȰѰ�"};
const unsigned char Pymb_ba[]    ={"�Ѱ˰ɰְΰհϰͰŰǰӰ԰Ȱаʰ̰ư�"};
const unsigned char Pymb_bai[]   ={"�ٰװڰܰذݰ۰ٲ�������"};
const unsigned char Pymb_ban[]   ={"��������߰�������������"};
const unsigned char Pymb_bang[]  ={"���������������������"};
const unsigned char Pymb_bao[]   ={"�����������������������ٰ���������������������"};
const unsigned char Pymb_bei[]   ={"������������������������������"};
const unsigned char Pymb_ben[]   ={"����������"};
const unsigned char Pymb_beng[]  ={"�ı��±ű���"};
const unsigned char Pymb_bi[]    ={"�ȱʱձǱ̱ұرܱƱϱ۱˱ɱӱѱݱбֱԱͱױα�ذ"};
const unsigned char Pymb_bian[]  ={"�߱����ޱ������"};
const unsigned char Pymb_biao[]  ={"��������"};
const unsigned char Pymb_bie[]   ={"�����"};
const unsigned char Pymb_bin[]   ={"����������"};
const unsigned char Pymb_bing[]  ={"������������������"};
const unsigned char Pymb_bo[]    ={"����������������������������������������"};
const unsigned char Pymb_bu[]    ={"��������������������"};
const unsigned char Pymb_c[]     ={"�Ŵγ������"};
const unsigned char Pymb_ca[]    ={"����"};
const unsigned char Pymb_cai[]   ={"�²ŲĲƲòɲʲǲȲ˲�"};
const unsigned char Pymb_can[]   ={"�βͲвϲѲҲ�"};
const unsigned char Pymb_cang[]  ={"�ֲײԲղ�"};
const unsigned char Pymb_cao[]   ={"�ٲڲܲ۲�"};
const unsigned char Pymb_ce[]    ={"���޲��"};
const unsigned char Pymb_ceng[]  ={"�����"};
const unsigned char Pymb_cha[]   ={"������������ɲ"};
const unsigned char Pymb_chai[]  ={"����"};
const unsigned char Pymb_chan[]  ={"�������������������"};
const unsigned char Pymb_chang[] ={"������������������������"};
const unsigned char Pymb_chao[]  ={"��������������������"};
const unsigned char Pymb_che[]   ={"������������"};
const unsigned char Pymb_chen[]  ={"�������������³��ĳ�"};
const unsigned char Pymb_cheng[] ={"�Ƴųɳʳгϳǳ˳ͳ̳γȳѳҳ�"};
const unsigned char Pymb_chi[]   ={"�Գճڳس۳ٳֳ߳޳ݳܳ����"};
const unsigned char Pymb_chong[] ={"������"};
const unsigned char Pymb_chou[]  ={"�������������"};
const unsigned char Pymb_chu[]   ={"����������������������������������"};
const unsigned char Pymb_chuai[] ={"��"};
const unsigned char Pymb_chuan[] ={"��������������"};
const unsigned char Pymb_chuang[]={"����������"};
const unsigned char Pymb_chui[]  ={"����������"};
const unsigned char Pymb_chun[]  ={"��������������"};
const unsigned char Pymb_chuo[]  ={"��"};
const unsigned char Pymb_ci[]    ={"�ôʴĴɴȴǴŴƴ˴δ̴�"};
const unsigned char Pymb_cong[]  ={"�ѴӴҴдϴ�"};
const unsigned char Pymb_cou[]   ={"��"};
const unsigned char Pymb_cu[]    ={"�ִٴ״�"};
const unsigned char Pymb_cuan[]  ={"�ڴܴ�"};
const unsigned char Pymb_cui[]   ={"�޴ߴݴ�����"};
const unsigned char Pymb_cun[]   ={"����"};
const unsigned char Pymb_cuo[]   ={"�������"};
const unsigned char Pymb_d[]     ={"�ĵ�ȶൽ��"};
const unsigned char Pymb_da[]    ={"�������"};
const unsigned char Pymb_dai[]   ={"������������������������"};
const unsigned char Pymb_dan[]   ={"������������������������������"};
const unsigned char Pymb_dang[]  ={"����������"};
const unsigned char Pymb_dao[]   ={"������������������������"};
const unsigned char Pymb_de[]    ={"�õµ�"};
const unsigned char Pymb_deng[]  ={"�Ƶǵŵȵ˵ʵ�"};
const unsigned char Pymb_di[]    ={"�͵̵εҵϵеӵѵյ׵ֵصܵ۵ݵڵ޵�"};
const unsigned char Pymb_dian[]  ={"���ߵ�������������"};
const unsigned char Pymb_diao[]  ={"�����������"};
const unsigned char Pymb_die[]   ={"��������������"};
const unsigned char Pymb_ding[]  ={"������������������"};
const unsigned char Pymb_diu[]   ={"��"};
const unsigned char Pymb_dong[]  ={"��������������������"};
const unsigned char Pymb_dou[]   ={"����������������"};
const unsigned char Pymb_du[]    ={"�����������¶ĶöʶŶǶȶɶ�"};
const unsigned char Pymb_duan[]  ={"�˶̶ζ϶ж�"};
const unsigned char Pymb_dui[]   ={"�ѶӶԶ�"};
const unsigned char Pymb_dun[]   ={"�ֶضն׶ܶ۶ٶ�"};
const unsigned char Pymb_duo[]   ={"��߶�޶��������"};
const unsigned char Pymb_e[]     ={"����������������"};
const unsigned char Pymb_en[]    ={"��"};
const unsigned char Pymb_er[]    ={"����������������"};
const unsigned char Pymb_f[]     ={"�ɷַ��編��"};
const unsigned char Pymb_fa[]    ={"����������������"};
const unsigned char Pymb_fan[]   ={"����������������������������������"};
const unsigned char Pymb_fang[]  ={"���������������·÷ķ�"};
const unsigned char Pymb_fei[]   ={"�ɷǷȷƷʷ˷̷ͷϷзη�"};
const unsigned char Pymb_fen[]   ={"�ַԷ׷ҷշӷطڷٷ۷ݷܷ޷߷�"};
const unsigned char Pymb_feng[]  ={"����������������"};
const unsigned char Pymb_fo[]    ={"��"};
const unsigned char Pymb_fou[]   ={"���"};
const unsigned char Pymb_fu[]    ={"����������������������������������������������������������������������������󸽸�������"};
const unsigned char Pymb_g[]     ={"�������߹���"};
const unsigned char Pymb_ga[]    ={"�¸�"};
const unsigned char Pymb_gai[]   ={"�øĸƸǸȸ�"};
const unsigned char Pymb_gan[]   ={"�ɸʸ˸θ͸ѸϸҸи̸�"};
const unsigned char Pymb_gang[]  ={"�Ըոڸٸظ׸ָ۸�"};
const unsigned char Pymb_gao[]   ={"��߸���ݸ޸���"};
const unsigned char Pymb_ge[]    ={"����������������������"};
const unsigned char Pymb_gei[]   ={"��"};
const unsigned char Pymb_gen[]   ={"����"};
const unsigned char Pymb_geng[]  ={"��������������"};
const unsigned char Pymb_gong[]  ={"������������������������������"};
const unsigned char Pymb_gou[]   ={"������������������"};
const unsigned char Pymb_gu[]    ={"�����ù¹��������ŹȹɹǹƹĹ̹ʹ˹�"};
const unsigned char Pymb_gua[]   ={"�Ϲιйѹҹ�"};
const unsigned char Pymb_guai[]  ={"�Թչ�"};
const unsigned char Pymb_guan[]  ={"�ع۹ٹڹ׹ݹܹ�߹��"};
const unsigned char Pymb_guang[] ={"����"};
const unsigned char Pymb_gui[]   ={"�����������������"};
const unsigned char Pymb_gun[]   ={"�����������"};
const unsigned char Pymb_guo[]   ={"����������������"};
const unsigned char Pymb_h[]     ={"�úͺ󻹻��"};
const unsigned char Pymb_ha[]    ={"���"};
const unsigned char Pymb_hai[]   ={"������������������"};
const unsigned char Pymb_han[]   ={"��������������������������������������"};
const unsigned char Pymb_hang[]  ={"������"};
const unsigned char Pymb_hao[]   ={"���������úºźƺ�"};
const unsigned char Pymb_he[]    ={"�ǺȺ̺ϺκͺӺҺ˺ɺԺкʺغֺպ�"};
const unsigned char Pymb_hei[]   ={"�ں�"};
const unsigned char Pymb_hen[]   ={"�ۺܺݺ�"};
const unsigned char Pymb_heng[]  ={"��ߺ���"};
const unsigned char Pymb_hong[]  ={"����������"};
const unsigned char Pymb_hou[]   ={"��������"};
const unsigned char Pymb_hu[]    ={"������������������������������������"};
const unsigned char Pymb_hua[]   ={"������������������"};
const unsigned char Pymb_huai[]  ={"����������"};
const unsigned char Pymb_huan[]  ={"�����������û»�������������"};
const unsigned char Pymb_huang[] ={"�ĻŻʻ˻ƻ̻ͻȻǻɻлλѻ�"};
const unsigned char Pymb_hui[]   ={"�һֻӻԻջػ׻ڻܻ������߻޻�ݻٻ�"};
const unsigned char Pymb_hun[]   ={"�������"};
const unsigned char Pymb_huo[]   ={"�������������"};
const unsigned char Pymb_j[]     ={"�ͽ������ż�"};
const unsigned char Pymb_ji[]    ={"���������������������������������������������������������������������ƼǼ��ͼ˼ɼ��ʼ����ȼü̼żļ¼�������"};
const unsigned char Pymb_jia[]   ={"�ӼмѼϼҼμԼռ׼ּؼۼݼܼټ޼�Ю"};
const unsigned char Pymb_jian[]  ={"����߼����������������������������������������������������"};
const unsigned char Pymb_jiang[] ={"��������������������������"};
const unsigned char Pymb_jiao[]  ={"���������������������ǽƽʽȽýŽ½��˽ɽнνϽ̽ѽ;���"};
const unsigned char Pymb_jie[]   ={"�׽Խӽսҽֽڽٽܽ��ݽ޽ؽ߽����������"};
const unsigned char Pymb_jin[]   ={"���������������������������������"};
const unsigned char Pymb_jing[]  ={"��������������������������������������������������"};
const unsigned char Pymb_jiong[] ={"����"};
const unsigned char Pymb_jiu[]   ={"�������žþľ��¾ƾɾʾ̾ξǾȾ;�"};
const unsigned char Pymb_ju[]    ={"�ӾоѾԾҾϾֽ۾վ׾ھپؾ�޾ܾ߾����ݾ��۾�"};
const unsigned char Pymb_juan[]  ={"��������"};
const unsigned char Pymb_jue[]   ={"��������������"};
const unsigned char Pymb_jun[]   ={"����������������������"};
const unsigned char Pymb_k[]     ={"�����ɿ�˿�"};
const unsigned char Pymb_ka[]    ={"������"};
const unsigned char Pymb_kai[]   ={"����������"};
const unsigned char Pymb_kan[]   ={"��������������"};
const unsigned char Pymb_kang[]  ={"��������������"};
const unsigned char Pymb_kao[]   ={"��������"};
const unsigned char Pymb_ke[]    ={"�����¿ƿÿſĿǿȿɿʿ˿̿Ϳ�"};
const unsigned char Pymb_ken[]   ={"�Ͽѿҿ�"};
const unsigned char Pymb_keng[]  ={"�Կ�"};
const unsigned char Pymb_kong[]  ={"�տ׿ֿ�"};
const unsigned char Pymb_kou[]   ={"�ٿڿۿ�"};
const unsigned char Pymb_ku[]    ={"�ݿ޿߿����"};
const unsigned char Pymb_kua[]   ={"������"};
const unsigned char Pymb_kuai[]  ={"�����"};
const unsigned char Pymb_kuan[]  ={"���"};
const unsigned char Pymb_kuang[] ={"�����������"};
const unsigned char Pymb_kui[]   ={"����������������������"};
const unsigned char Pymb_kun[]   ={"��������"};
const unsigned char Pymb_kuo[]   ={"��������"};
const unsigned char Pymb_l[]     ={"����������¥"};
const unsigned char Pymb_la[]    ={"��������������"};
const unsigned char Pymb_lai[]   ={"������"};
const unsigned char Pymb_lan[]   ={"������������������������������"};
const unsigned char Pymb_lang[]  ={"��������������"};
const unsigned char Pymb_lao[]   ={"������������������"};
const unsigned char Pymb_le[]    ={"������"};
const unsigned char Pymb_lei[]   ={"����������������������"};
const unsigned char Pymb_leng[]  ={"������"};
const unsigned char Pymb_li[]    ={"��������������������������������������������������������������������"};
const unsigned char Pymb_lian[]  ={"����������������������������"};
const unsigned char Pymb_liang[] ={"������������������������"};
const unsigned char Pymb_liao[]  ={"������������������������"};
const unsigned char Pymb_lie[]   ={"����������"};
const unsigned char Pymb_lin[]   ={"������������������������"};
const unsigned char Pymb_ling[]  ={"����������������������������"};
const unsigned char Pymb_liu[]   ={"����������������������"};
const unsigned char Pymb_long[]  ={"��������¡��¤¢£"};
const unsigned char Pymb_lou[]   ={"¦¥§¨ª©"};
const unsigned char Pymb_lu[]    ={"¶¬®«¯­±²°³½¼¸¹»µ·¾º´"};
const unsigned char Pymb_luan[]  ={"������������"};
const unsigned char Pymb_lue[]   ={"����"};
const unsigned char Pymb_lun[]   ={"��������������"};
const unsigned char Pymb_luo[]   ={"������������������������"};
const unsigned char Pymb_lv[]    ={"��¿������������������������"};
const unsigned char Pymb_m[]     ={"����û������"};
const unsigned char Pymb_ma[]    ={"������������������"};
const unsigned char Pymb_mai[]   ={"������������"};
const unsigned char Pymb_man[]   ={"����������á������"};
const unsigned char Pymb_mang[]  ={"æâäãçå"};
const unsigned char Pymb_mao[]   ={"èëìéêîíïðóñò"};
const unsigned char Pymb_me[]    ={"ô"};
const unsigned char Pymb_mei[]   ={"ûöõü÷ýúøùÿ��þ��������"};
const unsigned char Pymb_men[]   ={"������"};
const unsigned char Pymb_meng[]  ={"����������������"};
const unsigned char Pymb_mi[]    ={"����������������������������"};
const unsigned char Pymb_mian[]  ={"������������������"};
const unsigned char Pymb_miao[]  ={"����������������"};
const unsigned char Pymb_mie[]   ={"����"};
const unsigned char Pymb_min[]   ={"������������"};
const unsigned char Pymb_ming[]  ={"������������"};
const unsigned char Pymb_miu[]   ={"��"};
const unsigned char Pymb_mo[]    ={"����ġģĤĦĥĢħĨĩĭİĪįĮīĬ"};
const unsigned char Pymb_mou[]   ={"Ĳıĳ"};
const unsigned char Pymb_mu[]    ={"ĸĶĵķĴľĿ��ļĹĻ��Ľĺ��"};
const unsigned char Pymb_n[]     ={"������������"};
const unsigned char Pymb_na[]    ={"��������������"};
const unsigned char Pymb_nai[]   ={"����������"};
const unsigned char Pymb_nan[]   ={"������"};
const unsigned char Pymb_nang[]  ={"��"};
const unsigned char Pymb_nao[]   ={"����������"};
const unsigned char Pymb_ne[]    ={"��"};
const unsigned char Pymb_nei[]   ={"����"};
const unsigned char Pymb_nen[]   ={"��"};
const unsigned char Pymb_neng[]  ={"��"};
const unsigned char Pymb_ni[]    ={"����������������������"};
const unsigned char Pymb_nian[]  ={"��������������"};
const unsigned char Pymb_niang[] ={"����"};
const unsigned char Pymb_niao[]  ={"����"};
const unsigned char Pymb_nie[]   ={"��������������"};
const unsigned char Pymb_nin[]   ={"��"};
const unsigned char Pymb_ning[]  ={"��š������Ţ"};
const unsigned char Pymb_niu[]   ={"ţŤŦť"};
const unsigned char Pymb_nong[]  ={"ũŨŧŪ"};
const unsigned char Pymb_nu[]    ={"ūŬŭ"};
const unsigned char Pymb_nuan[]  ={"ů"};
const unsigned char Pymb_nue[]   ={"űŰ"};
const unsigned char Pymb_nuo[]   ={"ŲŵųŴ"};
const unsigned char Pymb_nv[]    ={"Ů"};
const unsigned char Pymb_o[]     ={"Ŷ��"};
const unsigned char Pymb_ou[]    ={"ŷŹŸŻżźŽ"};
const unsigned char Pymb_p[]     ={"ƽƬƷ��Ʊ��"};
const unsigned char Pymb_pa[]    ={"ſž����������"};
const unsigned char Pymb_pai[]   ={"������������"};
const unsigned char Pymb_pan[]   ={"����������������"};
const unsigned char Pymb_pang[]  ={"����������"};
const unsigned char Pymb_pao[]   ={"��������������"};
const unsigned char Pymb_pei[]   ={"������������������"};
const unsigned char Pymb_pen[]   ={"����"};
const unsigned char Pymb_peng[]  ={"����������������������������"};
const unsigned char Pymb_pi[]    ={"��������������Ƥ��ƣơ��ƢƥƦƨƧƩ"};
const unsigned char Pymb_pian[]  ={"Ƭƫƪƭ"};
const unsigned char Pymb_piao[]  ={"ƯƮưƱ"};
const unsigned char Pymb_pie[]   ={"ƲƳ"};
const unsigned char Pymb_pin[]   ={"ƴƶƵƷƸ"};
const unsigned char Pymb_ping[]  ={"ƹƽ��ƾƺƻ��ƿƼ"};
const unsigned char Pymb_po[]    ={"����������������"};
const unsigned char Pymb_pou[]   ={"��"};
const unsigned char Pymb_pu[]    ={"������������������������������"};
const unsigned char Pymb_q[]     ={"��ȥ��ȫ��ǰ"};
const unsigned char Pymb_qi[]    ={"������������������������������������������������������������������������"};
const unsigned char Pymb_qia[]   ={"��ǡǢ"};
const unsigned char Pymb_qian[]  ={"ǧǪǤǨǥǣǦǫǩǰǮǯǬǱǭǳǲǴǷǵǶǸ"};
const unsigned char Pymb_qiang[] ={"ǺǼǹǻǿǽǾ��"};
const unsigned char Pymb_qiao[]  ={"������������������������������"};
const unsigned char Pymb_qie[]   ={"����������"};
const unsigned char Pymb_qin[]   ={"����������������������"};
const unsigned char Pymb_qing[]  ={"��������������������������"};
const unsigned char Pymb_qiong[] ={"����"};
const unsigned char Pymb_qiu[]   ={"����������������"};
const unsigned char Pymb_qu[]    ={"����������������ȡȢȣȥȤ"};
const unsigned char Pymb_quan[]  ={"ȦȫȨȪȭȬȩȧȮȰȯ"};
const unsigned char Pymb_que[]   ={"Ȳȱȳȴȸȷȵȶ"};
const unsigned char Pymb_qun[]   ={"ȹȺ"};
const unsigned char Pymb_r[]     ={"������������"};
const unsigned char Pymb_ran[]   ={"ȻȼȽȾ"};
const unsigned char Pymb_rang[]  ={"ȿ��������"};
const unsigned char Pymb_rao[]   ={"������"};
const unsigned char Pymb_re[]    ={"����"};
const unsigned char Pymb_ren[]   ={"��������������������"};
const unsigned char Pymb_reng[]  ={"����"};
const unsigned char Pymb_ri[]    ={"��"};
const unsigned char Pymb_rong[]  ={"��������������������"};
const unsigned char Pymb_rou[]   ={"������"};
const unsigned char Pymb_ru[]    ={"��������������������"};
const unsigned char Pymb_ruan[]  ={"����"};
const unsigned char Pymb_rui[]   ={"������"};
const unsigned char Pymb_run[]   ={"����"};
const unsigned char Pymb_ruo[]   ={"����"};
const unsigned char Pymb_s[]     ={"������˵��ʱ"};
const unsigned char Pymb_sa[]    ={"������"};
const unsigned char Pymb_sai[]   ={"��������"};
const unsigned char Pymb_san[]   ={"����ɡɢ"};
const unsigned char Pymb_sang[]  ={"ɣɤɥ"};
const unsigned char Pymb_sao[]   ={"ɦɧɨɩ"};
const unsigned char Pymb_se[]    ={"ɫɬɪ"};
const unsigned char Pymb_sen[]   ={"ɭ"};
const unsigned char Pymb_seng[]  ={"ɮ"};
const unsigned char Pymb_sha[]   ={"ɱɳɴɰɯɵɶɷ��"};
const unsigned char Pymb_shai[]  ={"ɸɹ"};
const unsigned char Pymb_shan[]  ={"ɽɾɼ��ɺɿ������ɻ������������դ"};
const unsigned char Pymb_shang[] ={"����������������"};
const unsigned char Pymb_shao[]  ={"����������������������"};
const unsigned char Pymb_she[]   ={"������������������������"};
const unsigned char Pymb_shen[]  ={"��������������������������������ʲ"};
const unsigned char Pymb_sheng[] ={"��������ʤ����ʡʥʢʣ"};
const unsigned char Pymb_shi[]   ={"��ʬʧʦʭʫʩʨʪʮʯʱʶʵʰʴʳʷʸʹʼʻʺʿ��������ʾʽ������������������������������������"};
const unsigned char Pymb_shou[]  ={"��������������������"};
const unsigned char Pymb_shu[]   ={"������������������������������������������������������ˡ����������"};
const unsigned char Pymb_shua[]  ={"ˢˣ"};
const unsigned char Pymb_shuai[] ={"˥ˤ˦˧"};
const unsigned char Pymb_shuan[] ={"˩˨"};
const unsigned char Pymb_shuang[]={"˫˪ˬ"};
const unsigned char Pymb_shui[]  ={"˭ˮ˰˯"};
const unsigned char Pymb_shun[]  ={"˱˳˴˲"};
const unsigned char Pymb_shuo[]  ={"˵˸˷˶"};
const unsigned char Pymb_si[]    ={"˿˾˽˼˹˻˺����������������"};
const unsigned char Pymb_song[]  ={"����������������"};
const unsigned char Pymb_sou[]   ={"��������"};
const unsigned char Pymb_su[]    ={"����������������������"};
const unsigned char Pymb_suan[]  ={"������"};
const unsigned char Pymb_sui[]   ={"����������������������"};
const unsigned char Pymb_sun[]   ={"������"};
const unsigned char Pymb_suo[]   ={"����������������"};
const unsigned char Pymb_t[]     ={"����̫ͷ��ͬ"};
const unsigned char Pymb_ta[]    ={"����������̡̢̤̣"};
const unsigned char Pymb_tai[]   ={"̨̧̥̦̫̭̬̩̪"};
const unsigned char Pymb_tan[]   ={"̸̵̷̶̴̮̰̯̲̱̳̹̻̺̼̾̿̽"};
const unsigned char Pymb_tang[]  ={"��������������������������"};
const unsigned char Pymb_tao[]   ={"����������������������"};
const unsigned char Pymb_te[]    ={"��"};
const unsigned char Pymb_teng[]  ={"��������"};
const unsigned char Pymb_ti[]    ={"������������������������������"};
const unsigned char Pymb_tian[]  ={"����������������"};
const unsigned char Pymb_tiao[]  ={"������������"};
const unsigned char Pymb_tie[]   ={"������"};
const unsigned char Pymb_ting[]  ={"��͡����ͤͥͣͦͧ͢"};
const unsigned char Pymb_tong[]  ={"ͨͬͮͩͭͯͪͫͳͱͰͲʹ"};
const unsigned char Pymb_tou[]   ={"͵ͷͶ͸"};
const unsigned char Pymb_tu[]    ={"͹ͺͻͼͽͿ;��������"};
const unsigned char Pymb_tuan[]  ={"����"};
const unsigned char Pymb_tui[]   ={"������������"};
const unsigned char Pymb_tun[]   ={"��������"};
const unsigned char Pymb_tuo[]   ={"����������������������"};
const unsigned char Pymb_w[]     ={"��Ϊ��������"};
const unsigned char Pymb_wa[]    ={"��������������"};
const unsigned char Pymb_wai[]   ={"����"};
const unsigned char Pymb_wan[]   ={"����������������������������������"};
const unsigned char Pymb_wang[]  ={"��������������������"};
const unsigned char Pymb_wei[]   ={"Σ��΢κΡΪΤΧΥΦΨΩάΫΰαβγέίή��δλζηθξνιμεο"};
const unsigned char Pymb_wen[]   ={"��������������������"};
const unsigned char Pymb_weng[]  ={"������"};
const unsigned char Pymb_wo[]    ={"������������������"};
const unsigned char Pymb_wu[]    ={"����������������������������������������������������������"};
const unsigned char Pymb_x[]     ={"��С����ѧ��������������"};
const unsigned char Pymb_xi[]    ={"Ϧϫ����ϣ������Ϣ��Ϥϧϩ����ϬϡϪ��Ϩ����ϥϰϯϮϱϭϴϲϷϵϸ϶"};
const unsigned char Pymb_xia[]   ={"ϺϹϻ��Ͽ��ϾϽϼ������"};
const unsigned char Pymb_xian[]  ={"ϳ����������������������������������������������������"};
const unsigned char Pymb_xiang[] ={"����������������������������������������"};
const unsigned char Pymb_xiao[]  ={"����������������С��ТФ��ЧУЦХ"};
const unsigned char Pymb_xie[]   ={"ЩШЪЫЭавбгЯЬдйкжмелиз"};
const unsigned char Pymb_xin[]   ={"����о����п��н����"};
const unsigned char Pymb_xing[]  ={"����������������������������"};
const unsigned char Pymb_xiong[] ={"��������������"};
const unsigned char Pymb_xiu[]   ={"��������������������"};
const unsigned char Pymb_xu[]    ={"��������������������������������������"};
const unsigned char Pymb_xuan[]  ={"������������ѡѢѤѣ"};
const unsigned char Pymb_xue[]   ={"��ѥѦѨѧѩѪ"};
const unsigned char Pymb_xun[]   ={"ѫѬѰѲѮѱѯѭѵѶѴѸѷѳ"};
const unsigned char Pymb_y[]     ={"һ��Ҳ����Ҫ"};
const unsigned char Pymb_ya[]    ={"ѾѹѽѺѻѼ��ѿ����������������"};
const unsigned char Pymb_yan[]   ={"������������������������������������������������������������������"};
const unsigned char Pymb_yang[]  ={"����������������������������������"};
const unsigned char Pymb_yao[]   ={"��������ҢҦҤҥҡң��ҧҨҩҪҫԿ"};
const unsigned char Pymb_ye[]    ={"ҬҭүҮҲұҰҵҶҷҳҹҴҺҸ"};
const unsigned char Pymb_yi[]    ={"һ����ҽ��ҿҼҾ������������������������������������������������������������������������������������������"};
const unsigned char Pymb_yin[]   ={"������������������������������ӡ��ط��"};
const unsigned char Pymb_ying[]  ={"ӦӲӰӪӭӳӬӮӥӢӱӨӯӤӣӧөӫݺ"};
const unsigned char Pymb_yo[]    ={"Ӵ"};
const unsigned char Pymb_yong[]  ={"ӶӵӸӹӺӷ��ӽӾ��ӿ��Ӽӻ��"};
const unsigned char Pymb_you[]   ={"������������������������������������������"};
const unsigned char Pymb_yu[]    ={"����������������������������������������������������Ԧ����������ԡԤ������Ԣ��ԣ������ԥ"};
const unsigned char Pymb_yuan[]  ={"ԩԧԨԪԱ԰ԫԭԲԬԮԵԴԳԯԶԷԹԺԸ"};
const unsigned char Pymb_yue[]   ={"ԻԼ��������Ծ��Խ"};
const unsigned char Pymb_yun[]   ={"������������������������"};
const unsigned char Pymb_z[]     ={"������ֻ֮��"};
const unsigned char Pymb_za[]    ={"������զ"};
const unsigned char Pymb_zai[]   ={"����������������"};
const unsigned char Pymb_zan[]   ={"��������"};
const unsigned char Pymb_zang[]  ={"������"};
const unsigned char Pymb_zao[]   ={"����������������������������"};
const unsigned char Pymb_ze[]    ={"��������"};
const unsigned char Pymb_zei[]   ={"��"};
const unsigned char Pymb_zen[]   ={"��"};
const unsigned char Pymb_zeng[]  ={"������"};
const unsigned char Pymb_zha[]   ={"����������բագէթըե��"};
const unsigned char Pymb_zhai[]  ={"իժլ��խծկ"};
const unsigned char Pymb_zhan[]  ={"մձճղհնչյոշռսջվ��տպ"};
const unsigned char Pymb_zhang[] ={"��������������������������������"};
const unsigned char Pymb_zhao[]  ={"��������������������צ"};
const unsigned char Pymb_zhe[]   ={"����������������������"};
const unsigned char Pymb_zhen[]  ={"��������������������������������֡"};
const unsigned char Pymb_zheng[] ={"��������������������֤֣��֢"};
const unsigned char Pymb_zhi[]   ={"ְֱֲֳִֵֶַָֹֺֻּֽ֧֥֦֪֭֮֨֯֫֬֩��־������������ֿ������������������"};
const unsigned char Pymb_zhong[] ={"����������������������"};
const unsigned char Pymb_zhou[]  ={"����������������������������"};
const unsigned char Pymb_zhu[]   ={"������������������������������ס��ע��פ��ף��������"};
const unsigned char Pymb_zhua[]  ={"ץ"};
const unsigned char Pymb_zhuai[] ={"ק"};
const unsigned char Pymb_zhuan[] ={"רשת׫׭"};
const unsigned char Pymb_zhuang[]={"ױׯ׮װ׳״��ײ"};
const unsigned char Pymb_zhui[]  ={"׷׵׶׹׺׸"};
const unsigned char Pymb_zhun[]  ={"׻׼"};
const unsigned char Pymb_zhuo[]  ={"׿׾׽��������������"};
const unsigned char Pymb_zi[]    ={"����������������������������"};
const unsigned char Pymb_zong[]  ={"��������������"};
const unsigned char Pymb_zou[]   ={"��������"};
const unsigned char Pymb_zu[]    ={"����������������"};
const unsigned char Pymb_zuan[]  ={"׬����"};
const unsigned char Pymb_zui[]   ={"��������"};
const unsigned char Pymb_zun[]   ={"����"};
const unsigned char Pymb_zuo[]   ={"��������������"};
const unsigned char Pymb_space[] ={""};

/*"ƴ�����뷨��ѯ���,������ĸ������(index)"*/
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

/*��������ĸ������*/
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

    cInputStrLength=strlen((const char*)strInput_Pystr);    /*����ƴ��������*/
    if(*strInput_Pystr=='\0')return("");       /*���������ַ�����0*/

    for(i=0;i<cInputStrLength;i++)
        *(strInput_Pystr+i)|=0x20;             /*����ĸ��תΪСд*/

    if(*strInput_Pystr=='i')return("");        /*����ƴ������*/
    if(*strInput_Pystr=='u')return("");
    if(*strInput_Pystr=='v')return("");

    cpHZ=PyIndex_headletter[strInput_Pystr[0]-'a'];        /*������ĸ����*/
    cpHZedge=PyIndex_headletter[strInput_Pystr[0]-'a'+1];  /*����ָ�����*/

    strInput_Pystr++;                           /*ָ��ƴ�����ڶ�����ĸ*/
    while(cpHZ<cpHZedge)                         /*����������*/
    {
        for(i=0;i<cInputStrLength;i++)
        {
            if(*(strInput_Pystr+i)!=*((*cpHZ).Py+i))break;    /*������ĸ������,�˳�*/
        }
        if(i==cInputStrLength)        /*��ĸ��ȫ��*/
        {
            return (*cpHZ).Pymb;
        }
        cpHZ++;
    }
    return "";                      /*�޹�����*/
}

/*************************************************************************

                            ���ּ�ƴ�����뷨

*************************************************************************/
//άһ:ת�����֣�ά��:��1:��ʼ��ĸ,ά��:�����ּ���Ӧ��ĸ����
static const unsigned char Num2Letter[8][2] = {
   'a',3,/*2*/   'd',3,/*3*/  'g',3, /*4*/  'j',3,/*5*/
   'm',3,/*6*/   'p',4,/*7*/  't',3, /*8*/  'w',4/*9*/
};

//------------------------------������Ӧ����--------------------------------
//����ʱ���ô˺���
//�����ҵ�����ĸ��
//�����ؿ�,��ʾ�˴�û�ҵ�
__arm const struct _PyIndex **NumKeyPyIme_pKey(PinYinIME_t *pPy,
                                         unsigned char *pFindCount,//�����ҵ��Ĺ���      
                                         unsigned char Num)   //����İ���ֵ,��Ӧ2-9,�˸��
{
  
  unsigned char NumCount = pPy->NumCount;
  unsigned char PrvFindCount = pPy->PerFindCount[NumCount - 1];//�ϴ��ҵ�����
  const struct _PyIndex **pPyTblIndex = pPy->pPyTblIndex + pPy->CurPos;//�ϴ�λ��
  //����Ƿ����˸��,���������һ������,ָ�����ϴ�λ��
  if(Num == ' '){
    if(NumCount >= 2){ //������������ĸʱ,��һ������
      NumCount--;
      pPy->NumCount = NumCount;
      PrvFindCount = pPy->PerFindCount[NumCount - 1];//���ϴε��ҵ�����
      pPy->PerFindCount[NumCount] = 0;  //�����¿�ʼ��
      *pFindCount = PrvFindCount;//�ϴθ���
      pPy->CurPos -= PrvFindCount;
      return pPyTblIndex -= PrvFindCount;  //�����ϴβ鵽��λ�õ�ֵ
    }
    else{//������һ����ĸ���δ����ʱ
       pPy->NumCount = 0;
       return NULL; //�����
    }
  }
  
  //��ֵ����ʱ
  if((Num < '2') || (Num > '9')){
    if(NumCount){
     *pFindCount = PrvFindCount;
     return pPyTblIndex;  //����λ�ò���
    }
   else return NULL;  //��û���������
  }
  
  unsigned char Letter = Num2Letter[Num - '2'][0];
  unsigned char LetterCount = Num2Letter[Num - '2'][1];
  //=====================�����һ����ĸʱ,�����׸���=========================
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
    pPy->NumCount += 1;    //������ȷ
    return pPyTblIndex;
  }
  //============================�������ֹ���ʱ============================
  else if(NumCount > 6){
     *pFindCount = PrvFindCount;
     return pPyTblIndex;  //����λ�ò���
  }
  
  //========================������ʱ,���ϴεĻ����ϼ�����ѯ��===============
  unsigned char PrvLetterPos = NumCount - 1; //�ϴ���ĸ����λ��
  
  const struct _PyIndex **pPyTblNewIndex = //���ҵ����ӵ���λ��,��ʱNumCount����������ֵ
    pPyTblIndex + pPy->PerFindCount[PrvLetterPos];  
  
  //���ϴλ����ϲ���,��ƴ����ĸ�������ʽ���в���
  unsigned char CurFindCount = 0; //�ҵ�����
  for(unsigned char j = 0; j < PrvFindCount; j++){
    const struct _PyIndex *pPyCurIndex = *(pPyTblIndex + j); //�ϴ�λ�ó�ʼ��
    unsigned char PrvLetter = *(pPyCurIndex->Py + PrvLetterPos);//�ϴε���ĸ
    //�ϴ���ȫ����ʱ�ϴξͲ����������
    if(*(pPyCurIndex->Py + NumCount) == '\0')
       pPyCurIndex++; 
   //�ڵ�ǰλ���н��ַ�λ���ƶ�����������λ��
    while(*(pPyCurIndex->Py + PrvLetterPos) == PrvLetter){
      if(*(pPyCurIndex->Py + NumCount) >= Letter) break;
      pPyCurIndex++; 
    }
   //�ڱ������ֶ�Ӧ���ַ�����ǰ����
   for(unsigned char CurLetter = Letter; 
       CurLetter < (Letter + LetterCount); CurLetter++){
     const unsigned char *pString = pPyCurIndex->Py; 
     if(*(pString + PrvLetterPos) != PrvLetter) break;//�ϴ��־Ͳ����ϣ�ֱ�ӵ��߽���
     int Find1stFlag = 0;  //�ҵ����ֱ�־
     while(*(pString + NumCount) == CurLetter){//���ζ�Ӧʱ,�����η��ϵĲ�����
       if(!Find1stFlag){//�ҵ���һ��ʱ
          CurFindCount++;
          *pPyTblNewIndex++ = pPyCurIndex; //�ҵ�����һ��,��������
          Find1stFlag = -1;   //�����ҵ�
        }
        pPyCurIndex++;
        pString = pPyCurIndex->Py;
     }
   }//end for i;    
  }//end for j;
  
  if(CurFindCount){//���ҵ�ʱ
    *pFindCount = CurFindCount;   //�ҵ�����
    pPy->PerFindCount[NumCount] = CurFindCount;
    pPy->CurPos += PrvFindCount;
    pPy->NumCount++;
    return pPyTblIndex + PrvFindCount;
  }
  //�����ϴβ������
  *pFindCount = PrvFindCount;
  return pPyTblIndex;  //����λ�ò���
}

//------------------------------�õ���ǰ��ĸ��--------------------------------
//�����ҵ�����ĸ��
const struct _PyIndex **NumKeyPyIme_pGetCur(const PinYinIME_t *pPy)
{
  return (const struct _PyIndex **)(&(pPy->pPyTblIndex[pPy->CurPos]));
}

//------------------------------�õ���ǰ��ĸ������--------------------------------
unsigned char NumKeyPyIme_GetCurCount(const PinYinIME_t *pPy)
{
  return pPy->PerFindCount[pPy->NumCount - 1];
}
                                                      


