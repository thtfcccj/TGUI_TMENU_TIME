/*
           pyInput.c - ƴ�����뷨ģ��

           ԭ��:�� ��
           ��д:�� ǿ(mail2li@21cn.com)
           �ظ�д��dap_ccj
           �����ο���https://blog.csdn.net/zerokkqq/article/details/6413694
*/

#include <string.h>
#include <stdio.h>

#include "PinYinIME.h"

/* "ƴ�����뷨�������б�,���(mb)" */
static const unsigned char Pymb_a[]     ={"����߹�����"};
static const unsigned char Pymb_ai[]    ={"�������������������������������������������"};
static const unsigned char Pymb_an[]    ={"�����������������������������������"};
static const unsigned char Pymb_ang[]   ={"������"};
static const unsigned char Pymb_ao[]    ={"�������������°ð���������������������������"};

static const unsigned char Pymb_b[]     ={"�ɰ˲����ȰѰ�"};
static const unsigned char Pymb_ba[]    ={"�ŰưǰȰɰʰ˰̰ͰΰϰаѰҰӰ԰հ���������������"};
static const unsigned char Pymb_bai[]   ={"�װذٰڰ۰ܰݰ޲�����"};
static const unsigned char Pymb_ban[]   ={"�߰��������������������������"};
static const unsigned char Pymb_bang[]  ={"���������������������"};
static const unsigned char Pymb_bao[]   ={"�����������������������������������������������������"};
static const unsigned char Pymb_bei[]   ={"���������������������������������������������������������"};
static const unsigned char Pymb_ben[]   ={"���������������"};
static const unsigned char Pymb_beng[]  ={"�������±ñı�ܡ���"};
static const unsigned char Pymb_bi[]    ={"�ƱǱȱɱʱ˱̱ͱαϱбѱұӱԱձֱױرٱڱ۱ܱ�����ذ��������ݩ��޵�������������������������������������������"};
static const unsigned char Pymb_bian[]  ={"�ޱ߱������������������������������������"};
static const unsigned char Pymb_biao[]  ={"�������������������������"};
static const unsigned char Pymb_bie[]   ={"�������"};
static const unsigned char Pymb_bin[]   ={"������������������������������"};
static const unsigned char Pymb_bing[]  ={"������������������������������"};
static const unsigned char Pymb_bo[]    ={"�ذٰ�����������������������������������������������ެ�������������������"};
static const unsigned char Pymb_bu[]    ={"��������������������������߲�����������"};

static const unsigned char Pymb_c[]     ={"�Ŵγ������"};
static const unsigned char Pymb_ca[]    ={"������"};
static const unsigned char Pymb_cai[]   ={"�²òĲŲƲǲȲɲʲ˲�"};
static const unsigned char Pymb_can[]   ={"�ͲβϲвѲҲ����������"};
static const unsigned char Pymb_cang[]  ={"�Բղֲײ���"};
static const unsigned char Pymb_cao[]   ={"�ٲڲ۲ܲ�ܳ�������"};
static const unsigned char Pymb_ce[]    ={"�޲߲�����"};
static const unsigned char Pymb_cen[]   ={"����"};
static const unsigned char Pymb_ceng[]  ={"�������"};
static const unsigned char Pymb_cha[]   ={"������������ɲ���������������������"};
static const unsigned char Pymb_chai[]  ={"�����٭�������"};
static const unsigned char Pymb_chan[]  ={"������������������������������������������������"};
static const unsigned char Pymb_chang[] ={"�������������������������������������������������"};
static const unsigned char Pymb_chao[]  ={"�������������������½�������"};
static const unsigned char Pymb_che[]   ={"�����������������"};
static const unsigned char Pymb_chen[]  ={"���������������³óĳ����������������"};
static const unsigned char Pymb_cheng[] ={"�ųƳǳȳɳʳ˳̳ͳγϳгѳҳ�ʢة�����������������������"};
static const unsigned char Pymb_chi[]   ={"�Գճֳ׳سٳڳ۳ܳݳ޳߳��������ܯ��߳������������������������������"};
static const unsigned char Pymb_chong[] ={"��������������������"};
static const unsigned char Pymb_chou[]  ={"�������������ٱ������"};
static const unsigned char Pymb_chu[]   ={"����������������������������������ءۻ�������������������"};
static const unsigned char Pymb_chuai[] ={"����������"};
static const unsigned char Pymb_chuan[] ={"������������������������"};
static const unsigned char Pymb_chuang[]={"����������������"};
static const unsigned char Pymb_chui[]  ={"��������������"};
static const unsigned char Pymb_chun[]  ={"��������������ݻ����"};
static const unsigned char Pymb_chuo[]  ={"������������"};
static const unsigned char Pymb_ci[]    ={"��ôĴŴƴǴȴɴʴ˴̴ʹ�����������������"};
static const unsigned char Pymb_cong[]  ={"�ϴдѴҴӴ������������"};
static const unsigned char Pymb_cou[]   ={"������"};
static const unsigned char Pymb_cu[]    ={"�ִ״ش��������������"};
static const unsigned char Pymb_cuan[]  ={"�ڴ۴�����ߥ����"};
static const unsigned char Pymb_cui[]   ={"�ݴ޴ߴ����������������"};
static const unsigned char Pymb_cun[]   ={"��������"};
static const unsigned char Pymb_cuo[]   ={"��������������������������"};

static const unsigned char Pymb_d[]     ={"�ĵ�ȶൽ��"};
static const unsigned char Pymb_da[]    ={"��������������������������"};
static const unsigned char Pymb_dai[]   ={"�������������������������ܤ߰߾����������"};
static const unsigned char Pymb_dan[]   ={"��������������������������������ʯ��������������������"};
static const unsigned char Pymb_dang[]  ={"����������������������"};
static const unsigned char Pymb_dao[]   ={"��������������������������߶���������"};
static const unsigned char Pymb_de[]    ={"�µõĵ��"};
static const unsigned char Pymb_deng[]  ={"�εŵƵǵȵɵʵ������������"};
static const unsigned char Pymb_di[]    ={"�ĵ̵͵εϵеѵҵӵԵյֵ׵صٵڵ۵ܵݵ���ص��ڮ��ۡ��ݶ��������������������"};
static const unsigned char Pymb_dia[]   ={"��"};
static const unsigned char Pymb_dian[]  ={"�ߵ�������������������������������"};
static const unsigned char Pymb_diao[]  ={"��������������������"};
static const unsigned char Pymb_die[]   ={"����������������ܦ�����������"};
static const unsigned char Pymb_ding[]  ={"������������������������������������"};
static const unsigned char Pymb_diu[]   ={"����"};
static const unsigned char Pymb_dong[]  ={"��������������������������������"};
static const unsigned char Pymb_dou[]   ={"����������������������"};
static const unsigned char Pymb_du[]    ={"�������������¶öĶŶƶǶȶɶ�ܶ�������������"};
static const unsigned char Pymb_duan[]  ={"�˶̶Ͷζ϶������"};
static const unsigned char Pymb_dui[]   ={"�ѶҶӶ�����"};
static const unsigned char Pymb_dun[]   ={"�նֶ׶ضٶڶ۶ܶ��������������"};
static const unsigned char Pymb_duo[]   ={"�ȶ޶߶�������������������������"};

static const unsigned char Pymb_e[]     ={"������������������Ŷج������ݭ��������������������"};
static const unsigned char Pymb_ei[]    ={"��"};
static const unsigned char Pymb_en[]    ={"��������"};
static const unsigned char Pymb_er[]    ={"�������������������������"};

static const unsigned char Pymb_f[]     ={"�ɷַ��編��"};
static const unsigned char Pymb_fa[]    ={"��������������������"};
static const unsigned char Pymb_fan[]   ={"����������������������������������ެ����������"};
static const unsigned char Pymb_fang[]  ={"���������������·÷ķ�������������"};
static const unsigned char Pymb_fei[]   ={"�ƷǷȷɷʷ˷̷ͷηϷз�����������������������������������"};
static const unsigned char Pymb_fen[]   ={"�ҷӷԷշַ׷طٷڷ۷ܷݷ޷߷��������������"};
static const unsigned char Pymb_feng[]  ={"����������������ٺۺ�������"};
static const unsigned char Pymb_fo[]    ={"��"};
static const unsigned char Pymb_fou[]   ={"���"};
static const unsigned char Pymb_fu[]    ={"�����������������������������������������������������������������������������������������������ۮܽ������ݳ����߻����������������������������������������������"};

static const unsigned char Pymb_g[]     ={"�������߹���"};
static const unsigned char Pymb_ga[]    ={"���¼п�٤���������"};
static const unsigned char Pymb_gai[]   ={"�øĸŸƸǸȽ�ؤ���������"};
static const unsigned char Pymb_gan[]   ={"�ɸʸ˸̸͸θϸиѸҸ�������ߦ���������������"};
static const unsigned char Pymb_gang[]  ={"�Ըոָ׸ظٸڸ۸ܿ�����"};
static const unsigned char Pymb_gao[]   ={"�ݸ޸߸�������غھ۬޻��������"};
static const unsigned char Pymb_ge[]    ={"�Ǹ���������������������Ͽ�ت������ܪ������������������"};
static const unsigned char Pymb_gei[]   ={"��"};
static const unsigned char Pymb_gen[]   ={"����بݢ����"};
static const unsigned char Pymb_geng[]  ={"�����������������������"};
static const unsigned char Pymb_gong[]  ={"���������������������������������������"};
static const unsigned char Pymb_gou[]   ={"��������������������ڸ������������������"};
static const unsigned char Pymb_gu[]    ={"�������������¹ùĹŹƹǹȹɹʹ˹̹ͼ���ڬ��������������������������������������"};
static const unsigned char Pymb_gua[]   ={"�ιϹйѹҹ���ڴ�������"};
static const unsigned char Pymb_guai[]  ={"�Թչ���"};
static const unsigned char Pymb_guan[]  ={"�׹عٹڹ۹ܹݹ޹߹������ݸ����������"};
static const unsigned char Pymb_guang[] ={"������������"};
static const unsigned char Pymb_gui[]   ={"�����������������Ȳ���������������������"};
static const unsigned char Pymb_gun[]   ={"�������������"};
static const unsigned char Pymb_guo[]   ={"������������������������������������"};

static const unsigned char Pymb_h[]     ={"�úͺ󻹻��"};
static const unsigned char Pymb_ha[]    ={"�����"};
static const unsigned char Pymb_hai[]   ={"������������������������"};
static const unsigned char Pymb_han[]   ={"��������������������������������������������������������"};
static const unsigned char Pymb_hang[]  ={"�����������������"};
static const unsigned char Pymb_hao[]   ={"�亾�������ºúĺźƺ���޶����������"};
static const unsigned char Pymb_he[]    ={"�ǺȺɺʺ˺̺ͺκϺкѺҺӺԺպֺ׺���ڭ�������������������"};
static const unsigned char Pymb_hei[]   ={"�ٺ�"};
static const unsigned char Pymb_hen[]   ={"�ۺܺݺ�"};
static const unsigned char Pymb_heng[]  ={"�ߺ����޿����"};
static const unsigned char Pymb_hong[]  ={"��������������ڧݦޮް����"};
static const unsigned char Pymb_hou[]   ={"��������ܩ��������������"};
static const unsigned char Pymb_hu[]    ={"�˺����������������������������������������������������������������������������������"};
static const unsigned char Pymb_hua[]   ={"�������������������������"};
static const unsigned char Pymb_huai[]  ={"������������"};
static const unsigned char Pymb_huan[]  ={"�������������������������»�ۨۼ����ߧ������������������"};
static const unsigned char Pymb_huang[] ={"�ĻŻƻǻȻɻʻ˻̻ͻλϻл��������������������"};
static const unsigned char Pymb_hui[]   ={"�һӻԻջֻ׻ػٻڻۻܻݻ޻߻���������ڶ����ޥ����������������������"};
static const unsigned char Pymb_hun[]   ={"�������ڻ��������"};
static const unsigned char Pymb_huo[]   ={"�ͻ��������������޽߫������������"};

static const unsigned char Pymb_j[]     ={"�ͽ������ż�"};
static const unsigned char Pymb_ji[]    ={"�������������������������������������������������������������������������������������¼üļżƼǼȼɼʼ˼̼ͽ���ϵآؽ����٥����ܸ����ݽ��ު��ߴ�����������������������������������������������������������"};
static const unsigned char Pymb_jia[]   ={"�μϼмѼҼӼԼռּ׼ؼټڼۼܼݼ�����٤ۣ��������������������������������"};
static const unsigned char Pymb_jian[]  ={"�߼���������������������������������������������������������������������������������������������������������������"};
static const unsigned char Pymb_jiang[] ={"�罩������������������������ǿ����������������"};
static const unsigned char Pymb_jiao[]  ={"�������������������������½ýĽŽƽǽȽɽʽ˽̽ͽνϽнѾ�Уٮ��ܴ����������������������"};
static const unsigned char Pymb_jie[]   ={"�ҽӽԽսֽ׽ؽٽڽ۽ܽݽ޽߽���������������ڦڵ�������������������������"};
static const unsigned char Pymb_jin[]   ={"�����������������������������������ݣ������������������������"};
static const unsigned char Pymb_jing[]  ={"������������������������������������������������������������ݼ������������������"};
static const unsigned char Pymb_jiong[] ={"����������"};
static const unsigned char Pymb_jiu[]   ={"���������¾þľžƾǾȾɾʾ˾̾;�����������������"};
static const unsigned char Pymb_ju[]    ={"����ϾоѾҾӾԾվ־׾ؾپھ۾ܾݾ޾߾����������ڪ��������������������������������������������"};
static const unsigned char Pymb_juan[]  ={"��������Ȧ۲�������������"};
static const unsigned char Pymb_jue[]   ={"���ŽǾ����������������������ܥާ���������������������"};
static const unsigned char Pymb_jun[]   ={"�������������������������������"};

static const unsigned char Pymb_k[]     ={"�����ɿ�˿�"};
static const unsigned char Pymb_ka[]    ={"��������������"};
static const unsigned char Pymb_kai[]   ={"�������������������������"};
static const unsigned char Pymb_kan[]   ={"��������������٩��ݨ������"};
static const unsigned char Pymb_kang[]  ={"��������������������"};
static const unsigned char Pymb_kao[]   ={"����������������"};
static const unsigned char Pymb_ke[]    ={"�ǿ����¿ÿĿſƿǿȿɿʿ˿̿Ϳ��������������������������������"};
static const unsigned char Pymb_ken[]   ={"�Ͽпѿ���"};
static const unsigned char Pymb_keng[]  ={"�ӿ������"};
static const unsigned char Pymb_kong[]  ={"�տֿ׿�������"};
static const unsigned char Pymb_kou[]   ={"�ٿڿۿ���ޢߵ����"};
static const unsigned char Pymb_ku[]    ={"�ݿ޿߿������ܥ����"};
static const unsigned char Pymb_kua[]   ={"������٨�"};
static const unsigned char Pymb_kuai[]  ={"��������ۦ������"};
static const unsigned char Pymb_kuan[]  ={"�����"};
static const unsigned char Pymb_kuang[] ={"�����������ڲڿ������������"};
static const unsigned char Pymb_kui[]   ={"����������������������ظ���������������������������"};
static const unsigned char Pymb_kun[]   ={"��������������������"};
static const unsigned char Pymb_kuo[]   ={"����������"};

static const unsigned char Pymb_l[]     ={"����������¥"};
static const unsigned char Pymb_la[]    ={"�������������������������"};
static const unsigned char Pymb_lai[]   ={"���������������������"};
static const unsigned char Pymb_lan[]   ={"����������������������������������������"};
static const unsigned char Pymb_lang[]  ={"��������������ݹ����������"};
static const unsigned char Pymb_lao[]   ={"�������������������������������������"};
static const unsigned char Pymb_le[]    ={"��������߷����"};
static const unsigned char Pymb_lei[]   ={"��������������������������ڳ������������"};
static const unsigned char Pymb_leng[]  ={"������ܨ�"};
static const unsigned char Pymb_li[]    ={"��������������������������������������������������������������������تٳٵ۪����ݰ��޼߿�����������������������������������������������������������"};
static const unsigned char Pymb_lian[]  ={"��������������������������������������������������"};
static const unsigned char Pymb_liang[] ={"������������������������ܮݹ�����"};
static const unsigned char Pymb_liao[]  ={"��������������������������ޤ������������"};
static const unsigned char Pymb_lie[]   ={"�������������������������"};
static const unsigned char Pymb_lin[]   ={"�����������������������������������������������"};
static const unsigned char Pymb_ling[]  ={"������������������������������۹����������������������"};
static const unsigned char Pymb_liu[]   ={"����������������������µ½����������������"};
static const unsigned char Pymb_lo[]    ={"��"};
static const unsigned char Pymb_long[]  ={"����������¡¢£¤Ū���������������"};
static const unsigned char Pymb_lou[]   ={"¥¦§¨©ª¶�����������������"};
static const unsigned char Pymb_lu[]    ={"��«¬­®¯°±²³´µ¶·¸¹º»¼½¾����ߣ������������������������������������"};
static const unsigned char Pymb_lv[]    ={"¿���������������������������������������"};
static const unsigned char Pymb_lue[]   ={"�����"};
static const unsigned char Pymb_luan[]  ={"��������������������"};
static const unsigned char Pymb_lun[]   ={"����������������"};
static const unsigned char Pymb_luo[]   ={"���������������������������������������������������������"};

static const unsigned char Pymb_m[]     ={"߼����û������"};
static const unsigned char Pymb_ma[]    ={"������������������ĦĨ��������"};
static const unsigned char Pymb_mai[]   ={"������������۽ݤ��"};
static const unsigned char Pymb_man[]   ={"������������������áܬ��������������"};
static const unsigned char Pymb_mang[]  ={"âãäåæç��������"};
static const unsigned char Pymb_mao[]   ={"èéêëìíîïðñòó���������������������"};
static const unsigned char Pymb_me[]    ={"ô"};
static const unsigned char Pymb_mei[]   ={"õö÷øùúûüýþÿ����������ݮ���������������"};
static const unsigned char Pymb_men[]   ={"�������������"};
static const unsigned char Pymb_meng[]  ={"��������������������ޫ��������������"};
static const unsigned char Pymb_mi[]    ={"������������������������������ڢ����������������������������"};
static const unsigned char Pymb_mian[]  ={"�����������������������������"};
static const unsigned char Pymb_miao[]  ={"������������������������������"};
static const unsigned char Pymb_mie[]   ={"����ؿ�����"};
static const unsigned char Pymb_min[]   ={"������������������������������"};
static const unsigned char Pymb_ming[]  ={"������������ڤ���������"};
static const unsigned char Pymb_miu[]   ={"����"};
static const unsigned char Pymb_mo[]    ={"��û��ġĢģĤĥĦħĨĩĪīĬĭĮįİ���������������������������"};
static const unsigned char Pymb_mou[]   ={"ıĲĳٰ����������"};
static const unsigned char Pymb_mu[]    ={"ģĲĴĵĶķĸĹĺĻļĽľĿ�����������������"};

static const unsigned char Pymb_n[]     ={"������������"};
static const unsigned char Pymb_na[]    ={"����������������������"};
static const unsigned char Pymb_nai[]   ={"����������ؾ٦ܵ����"};
static const unsigned char Pymb_nan[]   ={"����������������"};
static const unsigned char Pymb_nang[]  ={"��߭������"};
static const unsigned char Pymb_nao[]   ={"����������ث������������"};
static const unsigned char Pymb_ne[]    ={"����ګ"};
static const unsigned char Pymb_nei[]   ={"����"};
static const unsigned char Pymb_nen[]   ={"���"};
static const unsigned char Pymb_neng[]  ={"��"};
static const unsigned char Pymb_ni[]    ={"������������������������٣�����������������"};
static const unsigned char Pymb_nian[]  ={"��������������إ���������"};
static const unsigned char Pymb_niang[] ={"����"};
static const unsigned char Pymb_niao[]  ={"������������"};
static const unsigned char Pymb_nie[]   ={"��������������ؿ����������"};
static const unsigned char Pymb_nin[]   ={"��"};
static const unsigned char Pymb_ning[]  ={"��������šŢ�������"};
static const unsigned char Pymb_niu[]   ={"ţŤťŦ�������"};
static const unsigned char Pymb_nong[]  ={"ŧŨũŪٯ��"};
static const unsigned char Pymb_nou[]   ={"��"};
static const unsigned char Pymb_nu[]    ={"ūŬŭ��������"};
static const unsigned char Pymb_nv[]    ={"Ů�����"};
static const unsigned char Pymb_nue[]   ={"Űű"};
static const unsigned char Pymb_nuan[]  ={"ů"};
static const unsigned char Pymb_nuo[]   ={"��ŲųŴŵ�������"};

static const unsigned char Pymb_o[]     ={"Ŷ���"};
static const unsigned char Pymb_ou[]    ={"ŷŸŹźŻżŽ��ک�����"};

static const unsigned char Pymb_p[]     ={"ƽƬƷ��Ʊ��"};
static const unsigned char Pymb_pa[]    ={"�ǰ�žſ��������������"};
static const unsigned char Pymb_pai[]   ={"��������������ٽ����"};
static const unsigned char Pymb_pan[]   ={"�������������������������������������"};
static const unsigned char Pymb_pang[]  ={"����������������������"};
static const unsigned char Pymb_pao[]   ={"������������������������"};
static const unsigned char Pymb_pei[]   ={"�����������������������������"};
static const unsigned char Pymb_pen[]   ={"������"};
static const unsigned char Pymb_peng[]  ={"����������������������������ܡ�����"};
static const unsigned char Pymb_pi[]    ={"�ٷ�����������������ơƢƣƤƥƦƧƨƩا��������ۯ����ܱ��ߨ��������������������������"};
static const unsigned char Pymb_pian[]  ={"���ƪƫƬƭ��������������"};
static const unsigned char Pymb_piao[]  ={"ƮƯưƱ����������������"};
static const unsigned char Pymb_pie[]   ={"ƲƳد���"};
static const unsigned char Pymb_pin[]   ={"ƴƵƶƷƸ�������"};
static const unsigned char Pymb_ping[]  ={"��ƹƺƻƼƽƾƿ����ٷ�����"};
static const unsigned char Pymb_po[]    ={"��������������������������۶�����������"};
static const unsigned char Pymb_pou[]   ={"������"};
static const unsigned char Pymb_pu[]    ={"���������������������������������������������������"};

static const unsigned char Pymb_q[]     ={"��ȥ��ȫ��ǰ"};
static const unsigned char Pymb_qi[]    ={"����������������������������������������������������������������������������ؽ��ٹ��ܻ��������ޭ�������������������������������������������"};
static const unsigned char Pymb_qia[]   ={"����ǡǢ������"};
static const unsigned char Pymb_qian[]  ={"ǣǤǥǦǧǨǩǪǫǬǭǮǯǰǱǲǳǴǵǶǷǸ��ٻ����ܷ����ݡ������������������������"};
static const unsigned char Pymb_qiang[] ={"ǹǺǻǼǽǾǿ�������������������������"};
static const unsigned char Pymb_qiao[]  ={"��������������������������������ȸ��ڽ������������������"};
static const unsigned char Pymb_qie[]   ={"����������٤ۧ�����������"};
static const unsigned char Pymb_qin[]   ={"��������������������������������������������"};
static const unsigned char Pymb_qing[]  ={"���������������������������������������������������"};
static const unsigned char Pymb_qiong[] ={"�����������������"};
static const unsigned char Pymb_qiu[]   ={"�������������������ٴ��������������������������"};
static const unsigned char Pymb_qu[]    ={"����������������ȡȢȣȤȥ��ڰ۾����ޡ޾��������������������������"};
static const unsigned char Pymb_quan[]  ={"ȦȧȨȩȪȫȬȭȮȯȰڹ�����������������"};
static const unsigned char Pymb_que[]   ={"ȱȲȳȴȵȶȷȸ�����"};
static const unsigned char Pymb_qun[]   ={"ȹȺ����"};

static const unsigned char Pymb_r[]     ={"������������"};
static const unsigned char Pymb_ran[]   ={"ȻȼȽȾ������"};
static const unsigned char Pymb_rang[]  ={"ȿ�����������"};
static const unsigned char Pymb_rao[]   ={"�����������"};
static const unsigned char Pymb_re[]    ={"������"};
static const unsigned char Pymb_ren[]   ={"�����������������������������������"};
static const unsigned char Pymb_reng[]  ={"����"};
static const unsigned char Pymb_ri[]    ={"��"};
static const unsigned char Pymb_rong[]  ={"������������������������������"};
static const unsigned char Pymb_rou[]   ={"������������"};
static const unsigned char Pymb_ru[]    ={"����������������������޸������������"};
static const unsigned char Pymb_ruan[]  ={"������"};
static const unsigned char Pymb_rui[]   ={"��������ި����"};
static const unsigned char Pymb_run[]   ={"����"};
static const unsigned char Pymb_ruo[]   ={"����ټ��"};

static const unsigned char Pymb_s[]     ={"������˵��ʱ"};
static const unsigned char Pymb_sa[]    ={"������ئ�����"};
static const unsigned char Pymb_sai[]   ={"����������"};
static const unsigned char Pymb_san[]   ={"����ɡɢ�����"};
static const unsigned char Pymb_sang[]  ={"ɣɤɥ�����"};
static const unsigned char Pymb_sao[]   ={"ɦɧɨɩܣ��������"};
static const unsigned char Pymb_se[]    ={"��ɪɫɬ����"};
static const unsigned char Pymb_sen[]   ={"ɭ"};
static const unsigned char Pymb_seng[]  ={"ɮ"};
static const unsigned char Pymb_sha[]   ={"ɯɰɱɲɳɴɵɶɷɼ��������������"};
static const unsigned char Pymb_shai[]  ={"ɫɸɹ"};
static const unsigned char Pymb_shan[]  ={"����ɺɻɼɽɾɿ��������������������դ��ڨ۷������������������������������"};
static const unsigned char Pymb_shang[] ={"�������������������������"};
static const unsigned char Pymb_shao[]  ={"������������������������ۿ�����������"};
static const unsigned char Pymb_she[]   ={"�����������������������������������������"};
static const unsigned char Pymb_shen[]  ={"����������������������������������ʲڷ��ݷ�������������"};
static const unsigned char Pymb_sheng[] ={"��������������ʡʢʣʤʥ����������"};
static const unsigned char Pymb_shi[]   ={"��ʦʧʨʩʪʫʬʭʮʯʰʱʲʳʴʵʶʷʸʹʺʻʼʽʾʿ��������������������������������������������ֳ������ݪ��߱��������������������������"};
static const unsigned char Pymb_shou[]  ={"���������������������������"};
static const unsigned char Pymb_shu[]   ={"����������������������������������������������������������������ˡحٿ����������������������"};
static const unsigned char Pymb_shua[]  ={"ˢˣ�"};
static const unsigned char Pymb_shuai[] ={"��ˤ˥˦˧�"};
static const unsigned char Pymb_shuan[] ={"˨˩����"};
static const unsigned char Pymb_shuang[]={"˪˫ˬ����"};
static const unsigned char Pymb_shui[]  ={"˭ˮ˯˰˵��"};
static const unsigned char Pymb_shun[]  ={"˱˲˳˴"};
static const unsigned char Pymb_shuo[]  ={"��˵˶˷˸����������"};
static const unsigned char Pymb_si[]    ={"˹˺˻˼˽˾˿��������������������ٹ�����������������������������"};
static const unsigned char Pymb_song[]  ={"����������������ڡݿ����������"};
static const unsigned char Pymb_sou[]   ={"����������޴�������������"};
static const unsigned char Pymb_su[]    ={"�����������������������������������������"};
static const unsigned char Pymb_suan[]  ={"�������"};
static const unsigned char Pymb_sui[]   ={"��������������������������ݴ��������"};
static const unsigned char Pymb_sun[]   ={"������ݥ������"};
static const unsigned char Pymb_suo[]   ={"ɯ�������������������������������"};

static const unsigned char Pymb_t[]     ={"����̫ͷ��ͬ"};
static const unsigned char Pymb_ta[]    ={"����������̡̢̣̤������������������"};
static const unsigned char Pymb_tai[]   ={"̧̨̥̦̩̪̫̬̭ۢ޷������������"};
static const unsigned char Pymb_tan[]   ={"��̴̵̶̷̸̮̯̰̱̲̳̹̺̻̼̽̾̿۰��������������"};
static const unsigned char Pymb_tang[]  ={"�������������������������������������������������"};
static const unsigned char Pymb_tao[]   ={"����������������������ػ߶��������"};
static const unsigned char Pymb_te[]    ={"��߯���"};
static const unsigned char Pymb_teng[]  ={"����������"};
static const unsigned char Pymb_ti[]    ={"���������������������������������������������"};
static const unsigned char Pymb_tian[]  ={"����������������������������"};
static const unsigned char Pymb_tiao[]  ={"������������٬������������������"};
static const unsigned char Pymb_tie[]   ={"�����������"};
static const unsigned char Pymb_ting[]  ={"������ͣͤͥͦͧ͢͡��������������"};
static const unsigned char Pymb_tong[]  ={"ͨͩͪͫͬͭͮͯͰͱͲͳʹ١�����������������"};
static const unsigned char Pymb_tou[]   ={"͵Ͷͷ͸������"};
static const unsigned char Pymb_tu[]    ={"͹ͺͻͼͽ;Ϳ��������ܢݱ������"};
static const unsigned char Pymb_tuan[]  ={"���������"};
static const unsigned char Pymb_tui[]   ={"��������������"};
static const unsigned char Pymb_tun[]   ={"�����������������"};
static const unsigned char Pymb_tuo[]   ={"˵����������������������ر٢������������������������"};

static const unsigned char Pymb_w[]     ={"��Ϊ��������"};
static const unsigned char Pymb_wa[]    ={"�������������������"};
static const unsigned char Pymb_wai[]   ={"������"};
static const unsigned char Pymb_wan[]   ={"��������������������������������������ܹݸ��������������"};
static const unsigned char Pymb_wang[]  ={"�����������������������������"};
static const unsigned char Pymb_wei[]   ={"��Ρ΢ΣΤΥΦΧΨΩΪΫάέήίΰαβγδεζηθικλμνξο��������������ޱ���������������������������������"};
static const unsigned char Pymb_wen[]   ={"�������������������������������"};
static const unsigned char Pymb_weng[]  ={"��������޳"};
static const unsigned char Pymb_wo[]    ={"��������������������ݫ���������"};
static const unsigned char Pymb_wu[]    ={"������������������������������������������������������������أ����������������������������������������������"};

static const unsigned char Pymb_x[]     ={"��С����ѧ��������������"};
static const unsigned char Pymb_xi[]    ={"����������������������ϡϢϣϤϥϦϧϨϩϪϫϬϭϮϯϰϱϲϳϴϵ϶Ϸϸ��������ۭ��ݾ������������������������������������������������������������"};
static const unsigned char Pymb_xia[]   ={"ϹϺϻϼϽϾϿ���������������������������"};
static const unsigned char Pymb_xian[]  ={"ϳϴ��������������������������������������������������������ݲ޺������������������������������"};
static const unsigned char Pymb_xiang[] ={"������������������������������������������ܼ��������������"};
static const unsigned char Pymb_xiao[]  ={"����������������������СТУФХЦЧ���������������������"};
static const unsigned char Pymb_xie[]   ={"��ШЩЪЫЬЭЮЯабвгдежзийклмѪҶ��������ޯߢ���������������������"};
static const unsigned char Pymb_xin[]   ={"ноп��������������ضܰݷ��������"};
static const unsigned char Pymb_xing[]  ={"ʡ������������������������������������ߩ����"};
static const unsigned char Pymb_xiong[] ={"��������������ܺ"};
static const unsigned char Pymb_xiu[]   ={"������������������������������������"};
static const unsigned char Pymb_xu[]    ={"����������������������������������������ڼ����ޣ������������������"};
static const unsigned char Pymb_xuan[]  ={"ȯ������������ѡѢѣѤ����������������������������"};
static const unsigned char Pymb_xue[]   ={"��ѥѦѧѨѩѪ���������"};
static const unsigned char Pymb_xun[]   ={"��ѫѬѭѮѯѰѱѲѳѴѵѶѷѸ��ۨ����ݡަ޹���������������"};

static const unsigned char Pymb_y[]     ={"һ��Ҳ����Ҫ"};
static const unsigned char Pymb_ya[]    ={"ѹѺѻѼѽѾѿ������������������������������������������"};
static const unsigned char Pymb_yan[]   ={"Ǧ��������������������������������������������������������������������������ٲ����ڥ����۱۳ܾ�������������������������������������������"};
static const unsigned char Pymb_yang[]  ={"������������������������������������������������"};
static const unsigned char Pymb_yao[]   ={"����ű��������ҡҢңҤҥҦҧҨҩҪҫԿزسߺ��������������������������"};
static const unsigned char Pymb_ye[]    ={"а��ҬҭҮүҰұҲҳҴҵҶҷҸҹҺ������������������"};
static const unsigned char Pymb_yi[]    ={"��βһҼҽҾҿ������������������������������������������������������������������������������������������������������٫ڱ������ܲ����޲������߽߮�����������������������������������������������������������������"};
static const unsigned char Pymb_yin[]   ={"������������������������������ӡط۴����ܧ���������������������"};
static const unsigned char Pymb_ying[]  ={"ӢӣӤӥӦӧӨөӪӫӬӭӮӯӰӱӲӳ��۫����ݺ��������������������������"};
static const unsigned char Pymb_yo[]    ={"Ӵ���"};
static const unsigned char Pymb_yong[]  ={"ӵӶӷӸӹӺӻӼӽӾӿ��������ٸ��ܭ�����������"};
static const unsigned char Pymb_you[]   ={"��������������������������������������������٧ݬݯݵ��������������������������������"};
static const unsigned char Pymb_yu[]    ={"εξ������������������������������������������������������������������������������ԡԢԣԤԥԦخع��ٶ�������������������������������������������������������������������������������"};
static const unsigned char Pymb_yuan[]  ={"ԧԨԩԪԫԬԭԮԯ԰ԱԲԳԴԵԶԷԸԹԺܫܾ��������������������"};
static const unsigned char Pymb_yue[]   ={"��˵ԻԼԽԾԿ����������������������"};
static const unsigned char Pymb_yun[]   ={"Ա������������������������۩ܿ���������������"};

static const unsigned char Pymb_z[]     ={"������ֻ֮��"};
static const unsigned char Pymb_za[]    ={"��������զ����"};
static const unsigned char Pymb_zai[]   ={"������������������"};
static const unsigned char Pymb_zan[]   ={"���������������������"};
static const unsigned char Pymb_zang[]  ={"�����������"};
static const unsigned char Pymb_zao[]   ={"������������������������������"};
static const unsigned char Pymb_ze[]    ={"��������զ�����������������"};
static const unsigned char Pymb_zei[]   ={"��"};
static const unsigned char Pymb_zen[]   ={"����"};
static const unsigned char Pymb_zeng[]  ={"��������������"};
static const unsigned char Pymb_zha[]   ={"������������աբգդեզէըթ����߸����������������"};
static const unsigned char Pymb_zhai[]  ={"����ժիլխծկ���"};
static const unsigned char Pymb_zhan[]  ={"��հձղճմյնշոչպջռսվտ���������"};
static const unsigned char Pymb_zhang[] ={"����������������������������������۵��������"};
static const unsigned char Pymb_zhao[]  ={"����������������������צ��گ��������"};
static const unsigned char Pymb_zhe[]   ={"������������������������ߡ��������������"};
static const unsigned char Pymb_zhen[]  ={"��������������������������������֡����������������������������"};
static const unsigned char Pymb_zheng[] ={"����������������������֢֣֤ں����������"};
static const unsigned char Pymb_zhi[]   ={"ʶְֱֲֳִֵֶַָֹֺֻּֽ֥֦֧֪֭֮֨֩֫֬֯־ֿ��������������������������������ش��ۤ����������������������������������������������������������������"};
static const unsigned char Pymb_zhong[] ={"����������������������ڣ���������"};
static const unsigned char Pymb_zhou[]  ={"����������������������������ݧ����������������"};
static const unsigned char Pymb_zhu[]   ={"������������������������������������������������סעףפؼ��٪ۥ������������������������������������"};
static const unsigned char Pymb_zhua[]  ={"��ץצ"};
static const unsigned char Pymb_zhuai[] ={"קת"};
static const unsigned char Pymb_zhuan[] ={"��רשת׫׬׭�������"};
static const unsigned char Pymb_zhuang[]={"��׮ׯװױײ׳״���"};
static const unsigned char Pymb_zhui[]  ={"׵׶׷׸׹׺�������"};
static const unsigned char Pymb_zhun[]  ={"׻׼����"};
static const unsigned char Pymb_zhuo[]  ={"׽׾׿����������������پ��ߪ�����������"};
static const unsigned char Pymb_zi[]    ={"֨����������������������������������������������������������������������"};
static const unsigned char Pymb_zong[]  ={"����������������������"};
static const unsigned char Pymb_zou[]   ={"������������۸����"};
static const unsigned char Pymb_zu[]    ={"����������������������"};
static const unsigned char Pymb_zuan[]  ={"����߬����"};
static const unsigned char Pymb_zui[]   ={"��������ީ��"};
static const unsigned char Pymb_zun[]   ={"����ߤ����"};
static const unsigned char Pymb_zuo[]   ={"������������������������������������"};
static const unsigned char Pymb_space[] ={""};

/*"ƴ�����뷨��ѯ���,������ĸ������(index)"*/
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
const struct _PyIndex **NumKeyPyIme_pGetCur(PinYinIME_t *pPy,
                                         unsigned char *pFindCount)//�����ҵ��Ĺ��� 
{
  *pFindCount = pPy->PerFindCount[pPy->NumCount - 1];
  return &(pPy->pPyTblIndex[pPy->CurPos]);
}

