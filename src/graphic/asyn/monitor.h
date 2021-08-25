#pragma once


#include "graphic/core/graphic.h"
#include "core/synchronize.h"
#include "core/thread.h"
#include "datastruct/VSString.h"
#include "datastruct/VSArray.h"
#include "graphic/core/resource.h"
#include "datastruct/VSSafeQueue.h"
#include "graphic/controller/animtree/animset.h"


namespace zq{


struct FileOperation
{
	enum Type
	{
		Added,
		Removed, 
		Modified
	};
};

struct FileChangedArgs
{
	FileOperation::Type	Operation;
	VSString			FilePath;
	bool operator==(const FileChangedArgs& rhs) const;
};

inline bool FileChangedArgs::operator==(const FileChangedArgs& rhs) const
{
	return (this->Operation == rhs.Operation) &&
		(this->FilePath == rhs.FilePath);
}

class GRAPHIC_API VSResourceChangedHandler
{
public:
	VSResourceChangedHandler()
	{

	}
	virtual ~VSResourceChangedHandler() {}
	// 检测文件类型是否可以处理
	virtual bool CheckFileType(const VSString & path) = 0;
	// 处理文件更新事件
	virtual void OnFileChanged(const FileChangedArgs& args) = 0;
};

class GRAPHIC_API VSAnimChangedHandler : public VSResourceChangedHandler
{
			
public:
	VSAnimChangedHandler()
	{

	}
	virtual ~VSAnimChangedHandler()
	{

	}
	virtual bool CheckFileType(const VSString & path)
	{
		return path.GetSubStringIndex(VSAnim::ms_FileSuffix, 1) != -1;
	}
	/// 处理文件更新事件
	virtual void OnFileChanged(const FileChangedArgs& args)
	{

	}
};


class VSMonitor; 
class GRAPHIC_API VSFileMonitor : public Thread
{
public:
	VSFileMonitor(VSMonitor * pOwner);
	virtual ~VSFileMonitor();

	// 设置监视目录
	void SetDirectory(const VSString& s);
	// 取得监视目录
	const VSString & GetDirectory() const;

	// 设置是否使用相对路径
	void SetUseRelativePath(bool b);
	// 是否使用相对路径
	bool IsUseRelativePath() const;

private:
	virtual void Run();
	virtual const char* GetThreadName();

	void NotifyHandler();
private:
	static const size_t BufferSize = 4096;
	VSString m_directory;
	bool m_isUseRelativePath;
	unsigned char* m_pReadBuffer;
	VSMonitor * m_pOwner;
};


inline const VSString& VSFileMonitor::GetDirectory() const
{
	return m_directory;
}

inline void VSFileMonitor::SetDirectory(const VSString& s)
{
	m_directory = s;
}

inline void VSFileMonitor::SetUseRelativePath(bool b)
{
	m_isUseRelativePath = b;
}

inline bool VSFileMonitor::IsUseRelativePath() const
{
	return m_isUseRelativePath;
}
class GRAPHIC_API VSMonitor 
{
public:
	VSMonitor()
	{

	}
	virtual ~VSMonitor()
	{

	}
protected:
	friend class VSFileMonitor;
	virtual void OnFileChanged(const FileChangedArgs& args) = 0;
};
class GRAPHIC_API VSResourceMonitor : public VSMonitor
{
public:
	VSResourceMonitor();
	virtual ~VSResourceMonitor();		
	bool Open();		
	void Update(double dAppTime);	
	void Close();
	inline void RegisterHandler(VSResourceChangedHandler* handler)
	{
		m_handlers.AddElement(handler);
	}
	
	inline void UnregisterHandler(VSResourceChangedHandler* handler)
	{
		uint32 index = m_handlers.FindElement(handler);
		m_handlers.Erase(index);
	}
	static VSResourceMonitor * ms_pResourceMonitor;
protected:
	void OnFileChanged(const FileChangedArgs& args);

private:
	VSFileMonitor* m_fileMonitor;
	VSSafeQueue<FileChangedArgs> m_changedFiles;
	VSArray<VSResourceChangedHandler*> m_handlers;
};


};
