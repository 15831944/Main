#pragma once

#ifndef X_FRAMEWORK_FRAME_TOOLS_FRAME_GRID_H
#define X_FRAMEWORK_FRAME_TOOLS_FRAME_GRID_H

class frame_data_piece;
class CTHDataBase;
class frame_data;


// 表实例名
#define FRAME_FRAMEIN				_T("钢筋面信息")
#define FRAME_HOOKS					_T("骨架弯钩信息")
#define FRAME_HOOKS_SUB_TABLE		_T("指定控制点弯钩信息二级表")

///////////////////////////////////////////////////////////////////////////////////////
// 重要提示：
//   用手写代码写入的标签名与使用表格设计器填入的标签名不一样，手写表写入的列标签名
//   中，相当于表格设计器中填写的变量名，行标签名相当于表格设计器中的行标签，表格设计
//   器没有单元格标签可设，但手写代码可写入。
//   所以在使用表格设计器的时候，列标签是用于辅助制作示意图的，它和手写表的列标签不是
//   同种性质的东西，如果设置会和表格设计器的变量名冲突！！！(设置器逻辑上有点问题)
//   本DLL中的弯钩表使用表格设计器制作的弯钩表要响应消息使用其列标签（相当于表格中的
//   变量名），所以一定保证不要在表格设计器中设置标签值，而是要设置变量名，这里的宏
//   定义也使用的是变量名，只是为了思维上的一致性才把宏名叫作TAG_...
// 以上内容是因为使用冲突了，咨询饶玉成时他给的解释，注释之，备以后调试。【2011-07-15】
///////////////////////////////////////////////////////////////////////////////////////

#define TAG_REF_LINES_TOP			(_T("TAG_REF_LINES_NAME_TOP"))		// 参考线单元格索引标记
#define TAG_REF_LINES_BOT			(_T("TAG_REF_LINES_NAME_BOT"))		// 参考线单元格索引标记
#define TAG_IS_FRAME				(_T("TAG_IS_FRAME"))				// 是否是骨架单元格索引标记
#define TAG_FRAME_NAME				(_T("TAG_FRAME_NAME"))				// 骨架面标识单元格索引
#define TAG_HOOKS_SETTING			(_T("TAG_HOOKS_SETTING"))			// 指定弯钩信息
#define TAG_TOP						(_T("TAG_HAS_TOP"))					// 是否有顶层通长筋单元格索引
#define TAG_BOT						(_T("TAG_HAS_BOT"))					// 是否有底层通长筋单元格索引

#define TAG_HOOK_NAME				(_T("m_strIndex"))					// 弯钩表中的弯钩名(用)
#define TAG_HOOK_TYPE				(_T("m_strHookType"))				// 弯钩表中的弯钩类型
#define TAG_HOOK_LEN				(_T("m_strHookL"))					// 弯钩表中的弯钩长度
#define TAG_HOOK_ARC_R				(_T("m_strArcR"))					// 弯钩表中的弯钩圆弧半径
#define TAG_HOOK_DIRECT_TYPE		(_T("m_strEndDirect"))				// 弯钩表中的弯钩端部布置方向
#define TAG_HOOK_DIRECT_ANGLE		(_T("m_strDirectAngle"))			// 弯钩表中的弯钩的弯折角度
#define TAG_HOOK_NEG				(_T("m_strHookDirect"))				// 弯钩表中的弯钩弯折方向

#define HOOK_GENERAL				(_T("通用弯钩"))						// 通用弯钩的名称

// 用于格式修饰符
#define FORMAT_HOOKS_SETTING_NAME	(_T("指定弯钩所在的控制点序号"))
#define FORMAT_HOOKS_SETTING_ID		(_T("弯钩设置格式标识"))



class X_FRAMEWORK_API frame_grid
{
public:
	frame_grid(void);
	~frame_grid(void);

public:
	// 获取所有可注册的表名，且保留数组中原有数据
	static void GetAllInstNames(CStringArray& strTables,const CString& strPart = _T(""));
	
	// 获取骨架弯钩信息的表格实例名，如果需要使用自定义的实例名，请保证实例名以本函数的返回值开头
	static CString GetFrameInsts_Hooks(const CString& strPart = _T(""));
	
	// 获取骨架钢筋面的表格实例名，如果需要使用自定义的实例名，请保证实例名以本函数的返回值开头
	static CString GetFrameInsts_Frames(const CString& strPart = _T(""));
	
	// 获取弯钩设置信息的表格实例名，请保证实例名以本函数的返回值开头
	static CString GetFrameInsts_HookSettings(const CString& strPart = _T(""));

	// 创建钢筋面子表，并将表格返回
	// const std::vector<int> & vecRowNum
	//   每一个钢筋面中的子表行数，vector的size为钢筋面的个数，内部的元
	//   素值为对应的钢筋面中的控制点的预留行数。
	// bT为true表示启用界面钢筋反置参数，否则不启用
	// bD为true表示启用控制点直径，否则不启用
	// bEnableSteelType为true表示启用钢种号参数，否则不启用
	static CTHDataBase* create_frame_grid(const std::vector<int>& vecRowNum, bool bT = false, bool bD = false, bool bEnableSteelType = false);
	static CTHDataBase* create_frame_grid(bool bT = false, bool bD = false, bool bEnableSteelType = false); // 一个钢筋面，一个控制点

	// 初始化弯钩界面表格
	static void initlize_hooks_grid(CTHDataBase& hook_db );
	
	//设置弯钩表状态
	static void refresh_hook_cells_status(CTHDataBase& db);

	// 从表格中获取骨架数据
	static void get_frame_data_from_db(IN CTHDataBase& DBFrame, IN CTHDataBase& DBHooks, OUT frame_data& P0);
	
	// 加载骨架数据到表格
	static void set_frame_data_to_db(CTHDataBase& frame_db, CTHDataBase& hook_db, const frame_data& data);
};


#endif
