/* ----------------------------------------------------------------------------
 *                TGUI对外接口]
 * --------------------------------------------------------------------------*/

#ifndef __T_GUI_H 
#define __T_GUI_H

//注:TGUI内部不能调用此接口

//基本结构:
#include "TWin.h"       //窗口操作
#include "TWM.h"   //窗口管理器操作
#include "TWidget.h"   //用于扩展控件的基本构件

//TGUI实现的控件有:
//框架(已嵌入到Widget中),OK!
//水平与垂直滚动条(已嵌入到Widget中)OK!
//独立的列表框模式OK!
#include "TListbox.h"
//独立的扩展列表框模式,比普通列表框多更多的样式OK!
#include "TListboxEx.h"
//独立的单选框模式OK!
#include "TRadioButtom.h"
//独立的复选框模式OK!
#include "TCheckBox.h"
//独立的文本框:
#include "TText.h"
//编辑框:
#include "TEdit.h"

//多行编辑框(MutiEdit,基类为ListBox)替换单行编辑框Edit,可一次编辑多个内容
//进度条(■■■■□□□□□□□)//暂未实现

//未实现的功能:
//文本框(可用对话框代替)
//滑动条(数值调整代替,不需要实现)


#endif

