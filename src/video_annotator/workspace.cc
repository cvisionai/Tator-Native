#include "workspace.h"

#include <iostream>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

#include "mainwindow.h"
#include "playlist.h"

namespace tator
{
  Workspace::Workspace(QWidget *parent, Playlist * playlist) :
    QObject(parent),
    parent_(parent),
    currentIdx_(-1),
    newIdx_(-1),
    playlist_(playlist),
    progressDialog_(nullptr),
    fileState_(NOT_SAVED),
    seek_(false)
  {
    
  }
  
  void Workspace::validatePlaylist()
  {
    currentIdx_ = -1;
    const size_t count = playlist_->size();
    for (size_t idx = 0; idx < count; idx++)
    {
      Playlist::Status status =
	validateMP4JsonPair(playlist_->location(idx));
      
      playlist_->setStatus(idx, status);
    }
  }

  void Workspace::handleUserSelection(const QModelIndex &modelIdx)
  {
    int idx = modelIdx.row();
    updateToNewIdx(idx);
  }

  void Workspace::mediaLoaded()
  {
    // If we didn't make the selection exit.
    if (newIdx_ == -1)
    {
      return;
    }

    QString mp4File = playlist_->location(newIdx_);
    QString jsonFile = getJSONPath(mp4File);
    QFileInfo info(jsonFile);
    progressDialog_->setValue(50);
    if (info.exists())
    {
      progressDialog_->setLabelText(QString("Loading %1").arg(info.fileName()));
      emit requestLoadAnnotationFile(jsonFile);
    }
    else
    {
      progressDialog_.reset();
      emit error(QString("Couldn't load %1").arg(jsonFile));
    }

    //Update current
    currentIdx_ = newIdx_;
    fileState_ = NOT_SAVED;
    playlist_->setStatus(newIdx_, Playlist::IN_PROCESS);
  }

  void Workspace::annotationFileUpdated()
  {
    progressDialog_->setValue(100);
    progressDialog_->close();
    progressDialog_.reset();
  }

  void Workspace::annotationFileSaved()
  {
    if (currentIdx_ != -1)
    {
      fileState_ = SAVED;
      QString mp4File = playlist_->location(currentIdx_);
      Playlist::Status status = validateMP4JsonPair(mp4File);
      playlist_->setStatus(currentIdx_, status);

      if (seek_ == true)
      {
	// Gracefully handle when we are seeking
	seek_=false;
      }
      else
      {
	// Else autoplay
	updateToNewIdx(currentIdx_+1);
      }
    }
  }
  
  QString Workspace::getJSONPath(const QString &mp4FilePath)
  {
    const QString jsonExt = ".json";
    QFileInfo mp4Info(mp4FilePath);
    QString directory = mp4Info.absoluteDir().absolutePath();
    QString basename = mp4Info.baseName();
    return directory + "/" + basename + jsonExt;
  }

  QString Workspace::getCSVPath(const QString &mp4FilePath)
  {
    const QString jsonExt = ".csv";
    QFileInfo mp4Info(mp4FilePath);
    QString directory = mp4Info.absoluteDir().absolutePath();
    QString basename = mp4Info.baseName();
    return directory + "/" + basename + jsonExt;
  }

  Playlist::Status Workspace::validateMP4JsonPair(const QString &mp4FilePath)
  {
    Playlist::Status status = Playlist::ERROR;
    
    QString jsonFile = getJSONPath(mp4FilePath);
    QString csvFile = getCSVPath(mp4FilePath);
    
    QFileInfo jsonInfo(jsonFile);
    QFileInfo csvInfo(csvFile);
    /// ## Truth table ##
    ///
    /// | JsonFile   |   CSV File   |   Status      |
    /// |------------|--------------|---------------|
    /// |  NO        | <ANY>        |    Error      |
    /// |  YES       | NO           | Not Processed |
    /// |  YES       | Yes          | Processed     |
    ///
    if (jsonInfo.exists() != true)
    {
      status = Playlist::ERROR;
    }
    else
    {
      if (csvInfo.exists() == true)
      {
	status = Playlist::PROCESSED;
      }
      else
      {
	status = Playlist::NOT_PROCESSED;
      }
    }
    
    return status;
  }

  void Workspace::updateToNewIdx(int idx)
  {
    if (idx >= playlist_->size())
    {
      currentIdx_=-1;
      fileState_ = NOT_SAVED;
      return;
    }

    if (currentIdx_ != -1)
    {
      if (fileState_ == NOT_SAVED)
      {
	QString jsonFile = getJSONPath(playlist_->location(currentIdx_));
	QFileInfo jsonInfo(jsonFile);
	QString msg = QString("Must save changes for '%1' to continue.")
	  .arg(jsonInfo.fileName());
	auto result = QMessageBox::question(parent_,
					    "Proceed with save?", msg);
	if (result == QMessageBox::Yes)
	{
	  seek_=true;
	  emit requestFileSave();
	}
	else
	{
	  return;
	}
      }
      
      Playlist::Status status =
	validateMP4JsonPair(playlist_->location(currentIdx_));
      playlist_->setStatus(currentIdx_, status);
    }
    
    newIdx_ = idx;
    QString mp4File = playlist_->location(idx);
    QFileInfo mp4Info(mp4File);
    if (mp4Info.exists())
    {
      std::cout << " -- Workspace: Loading " << mp4File.toStdString()
		<< std::endl;
      progressDialog_ =
	std::unique_ptr<QProgressDialog>(
	  new QProgressDialog(QString("Loading %1").arg(mp4Info.fileName()),
			      QString(), //no cancel
			      0,
			      100,
			      parent_,
			      Qt::Popup));
      
      emit requestLoadVideo(mp4File);
    }
    else
    {
      emit error(QString("Couldn't load %1").arg(mp4File));
    }
    // handle rest in mediaLoaded slot
  }
}
