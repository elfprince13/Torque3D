#include "platformQT.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include "core/util/tVector.h"
#include "core/stringTable.h"
#include "console/console.h"
#include "core/strings/stringFunctions.h"
#include "util/tempAlloc.h"
#include "cinterface/cinterface.h"
#include "core/volume.h"

StringTableEntry osGetTemporaryDirectory()
{
	QString tmpPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	bool canUse = QDir::mkpath(tmpPath);
	QFile qf(tmpPath);
	AssertFatal(canUse,"No writeable tempdir");
	QByteArray byteArray = qf.encodeName();
	const char* cString = byteArray.constData();
	return StringTable->insert(cString);
}

QString qsFromPath(const char *path){
	QByteArray qba(path);
	return QFile::decodeName(qba);
}

bool dPathCopy(const char *fromName, const char *toName, bool nooverwrite)
{
	QStr fN = qsFromPath(fromName);
	QFile from(fN);
	QStr tN = qsFromPath(toName);
	QFile to(tN);
	if(!nooverwrite && to.exists()){
		bool ret = to.remove()
		if(!ret) return ret;
	}
	return from.copy(tN)
}

 //-----------------------------------------------------------------------------
 bool dFileDelete(const char * name)
{
	QFile qf(qsFromPath(name));
	return qf.remove();
}

 //-----------------------------------------------------------------------------
bool dFileTouch(const char * name)
{
	QFile file(qsFromPath(name));
	bool status = file.open(QIODevice::ReadWrite);
	if(status) file.close();
	return status;
}

 bool dFileRename(const char *oldName, const char *newName)
{
	AssertFatal( oldName != NULL && newName != NULL, "dFileRename - NULL file name" );
		
	QFile qf(qsFromPath(oldName));
	QString toName = qsFromPath(newName);
	return qf.rename(qf, toName);
}

