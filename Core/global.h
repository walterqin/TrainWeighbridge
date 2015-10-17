/**
* @file global.h
* @brief 定义项目中用到的全局数据，包括：
*        全局常数
*        通用API
*        全局变量
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-07
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include "core_global.h"
#include <QMessageBox>
#include <QMetaType>
#include <math.h>

const bool Use_Local_Func = false;
const bool Use_Dll_Func = true;

#define RAWFILENO 1
#define REFFILENO 2
#define FLTFILENO 3
#define DIFFILENO 4
#define IDXFILENO 5

const int conBufSize = 300000; //采集数据缓存最大长度
const int conFilterLen = 10;   //过滤区长度
const int conDifStep = 13;     //查分步长

const int conSampRate = 100;   //采样率
const int conKeepDataSize = 100; //上称后前面保留的数据长度
const int conRefPointDataSize = 25; //计算参考点数据长度
const int conRenewRefPointDataSize = 128; //更新参考点数据长度
const int conTrainOnDataSize = 13; //判断上称数据长度

#define LEFTDIRECTION 1
#define RIGHTDIRECTION 2

#define WHEELUP 1
#define WHEELDN -1

#define CARRIAGE4 1
#define WAGON4 2
#define ENGINE4 3
#define ENGINE6 4

class QSqlRecord;

namespace weighbridge
{
    /* 采样数据类型 */
    enum SampleType
    {
        SampleTypeNormal,
        sampleTypeCalibration,
        SampleTypeQc,
        SampleTypeSim,
    };

    /** 采样数据来源 */
    enum SampleResource
    {
        SampleResourceNormal,
        SampleResourceCalibration,
        SampleResourceSimulate,
    };

    /** 检测结果标志 */
    enum ResultFlag
    {
        RepeatResult			= 0x0001,	// 重测结果
        DilutionResult			= 0x0002,	// 稀释结果

        // 打印报告限, 当结果超出这个限时, 出报告时应该警示操作人员
        LowerReportLimit		= 0x0004,
        UpperReportLimit		= 0x0008,
        ReportLimitMask			= LowerReportLimit | UpperReportLimit,

        // 正常值标记限
        LowerMarkLimit			= 0x0010,
        UpperMarkLimit			= 0x0020,
        MarkLimitMask			= LowerMarkLimit | UpperMarkLimit,

        LowerRedilutionLimit	= 0x0040,
        UpperRedilutionLimit	= 0x0080,
        RedilutionLimitMask		= LowerRedilutionLimit | UpperRedilutionLimit,

        LowerRepeatLimit		= 0x0100,
        UpperRepeatLimit		= 0x0200,
        RepeatLimitMask			= LowerRepeatLimit | UpperRepeatLimit,

        ReplicationDiff			= 0x0400,	// 多次测量结果不一致
        EvolveResult			= 0x0800,	// 演算的结果
        ManualTestResult		= 0x1000,	// 手动测试结果
        ManualInputResult		= 0x8000,	// 用户输入的结果
    };
    Q_DECLARE_FLAGS(ResultFlags, ResultFlag)

    /** 分析优先级 */
    enum AnalysisPriority
    {
        HighPriority,
        NormalPriority,
        LowPriority,
    };

    /** 分析状态 */
    enum AnalysisStatus
    {
        Ready,			// 待测
        Reading,		// 读数
        Analysing,		// 分析
        Finished,		// 完成
        Validated,		// 只在数据库中使用
    };

    /** 质控方法 */
    enum QcMethod
    {

    };

    /** 质控标志 */
    enum QcFlag
    {

    };

    /** 结果标志 */
    enum ExamFlag
    {
        ExamReady,
        ExamValidated,
    };

    /** 函数类型 */
    enum Function
    {
        FunLinear,
        FunLog,
    };

    /** 回归方法 */
    enum RegressionMethod
    {
        RegLinear,				// 线性回归
        Reg4PL			= 4,	// 4-PL回归
    };

    enum ControlMode
    {
        ModeManual,
        ModeAutomatic,
        ModeSimulate,
        ModeCalibrate,
    };

    enum WeighBridgeType
    {
        WeighBridgeNonBreakSingle,
        WeighBridgeBreakSingle,
        WeighBridgeBreakTwo,
        WeighBridgeBreakThree,
        WeighBridgeStaic
    };
}
Q_DECLARE_METATYPE(weighbridge::AnalysisStatus)

#define _STR2(x) #x
#define _STR(x) _STR2(x)
#define FILE_POSITION __FILE__"("_STR(__LINE__)"):"

#define TRACE_LAST_ERROR(q) qWarning() << FILE_POSITION << (q).lastError()

#ifndef _countof
#define _countof(a) (sizeof(a)/sizeof((a)[0]))
#endif

#ifdef QT_NO_DEBUG
    #define QDEBUG(x)		qt_noop()
    #define QWARNING(x)		qt_noop()
#else
    #define QDEBUG(x)		qDebug() << x
    #define QWARNING(x)		qWarning() << __FILE__ << "(" << __LINE__ << "):" << x
#endif

#define PRINTABLE(s)		QString(s).toUtf8().constData()

inline int qcPackStopFlag(int flags) { return flags << 16; }
inline int qcExtractStopFlag(int flags) { return flags >> 16; }

CORE_EXPORT double fraction(const QString &expr, bool *ok = 0);
CORE_EXPORT QVariant eval(const QString &expr);
CORE_EXPORT QString samplePrefix(weighbridge::SampleType sampleType);
CORE_EXPORT double round(double val, int precision = 4, char format = 'g');
CORE_EXPORT void sleep(int ms);
CORE_EXPORT int nextSampleNo(const QDate &date);
CORE_EXPORT void setGeneratedValue(QSqlRecord &rec, const QString &field, const QVariant &value);
CORE_EXPORT unsigned short crc16(const void *buf, int len);

template<typename T> T square(T t) { return t*t; }
template<typename T> const T &clamp(const T &val, const T &min, const T &max)
{ return val < min ? min : val > max ? max : val; }	// see qBound
template<typename T> void qDelete(T &p) { if (p) { delete p; p = 0; } }
template<typename T> int sign(const T &t) { return t >= 0 ? 1 : -1; }

// 计算样本偏差和平均值
template<typename T>
double stdev(const T *dat, int n, double *mean = 0)
{
    double m = 0;

    for(int i=0; i<n; i++)
        m += dat[i];
    m /= n;
    if (mean)
        *mean = m;

    double s = 0;

    for(int i=0; i<n; i++)
        s += square(dat[i] - m);
    s /= n;// - 1;
    s = sqrt(s);

    return s;
}

// 不确定度计算
template<typename T>
double uncertainty(const T *dat, int n, double *mean = 0)
{
    double m = 0;

    for(int i=0; i<n; i++)
        m += dat[i];
    m /= n;
    if (mean)
        *mean = m;

    double s = 0;

    for(int i=0; i<n; i++)
        s += square(dat[i] - m);
    s /= n - 1;
    s = sqrt(s);

    return s / qAbs(m);
}

CORE_EXPORT QWidget *topLevelWidget();

//信息提示函数
CORE_EXPORT QMessageBox::StandardButton
information(const QString &text,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
            QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

CORE_EXPORT QMessageBox::StandardButton
question(const QString &text,
         QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

CORE_EXPORT QMessageBox::StandardButton
warning(const QString &text,
        QMessageBox::StandardButtons buttons = QMessageBox::Ok,
        QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

CORE_EXPORT QMessageBox::StandardButton
critical(const QString &text,
         QMessageBox::StandardButtons buttons = QMessageBox::Ok,
         QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

template<class T>
class VariableHolder
{
public:
    VariableHolder(T &var, const T &val = T()) : m_var(var), m_val(var)
    {
        m_var = val;
    }
    ~VariableHolder()
    {
        restore();
    }
    void restore()
    {
        m_var = m_val;
    }

    T & m_var;
    const T m_val;
};

class CORE_EXPORT BoolLocker
{
public:
    BoolLocker(bool &b, bool val = true) : m_b(b)
    {
        m_val = m_b;
        m_b = val;
    }
    ~BoolLocker() { m_b = m_val; }
    bool &m_b;
    bool m_val;
};

class CORE_EXPORT WaitCursor
{
public:
    WaitCursor(Qt::CursorShape cursor = Qt::WaitCursor);
    ~WaitCursor();
};

#endif // GLOBAL_H

