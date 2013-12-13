/* 
 * File:   association.h
 * Author: gkr
 *
 * Created on 13 décembre 2013, 11:33
 */

#ifndef ASSOCIATION_H
#define	ASSOCIATION_H

class association {
public:

  /*! \brief Constructor
   *
   * Creates a new page association.
   */
  association(Echoes::Dbo::Session *session, std::string apiUrl);
  /**
   * In this methode you can call API for recover resources. \n
   * She is call after initialization of this class.
   */
  void          recoverListAsset();

protected:
  /*! \brief Closes the registration widget. 
   * 
   * The default implementation simply deletes the widget.
   */
  virtual void                          close();

  virtual void                          closePopup();

  virtual void                          update();
  virtual void                          modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);
  virtual void                          addResource(std::vector<Wt::WInteractWidget*> argument);
  virtual Wt::WDialog                   *deleteResource(long long id);
  
  virtual Wt::WValidator                *editValidator(int who);
  virtual void                          popupAddWidget(Wt::WDialog *dialog, bool typeDial);

  virtual std::vector<long long>        getIdsTable();
  virtual std::vector<std::string>      getTitlesTableText();
  virtual std::vector<std::string>      getTitlesTableWidget();
  virtual vector_type                   getResourceRowTable(long long id);

private:
  /*
   * Generate and get script (sonde) for asset.
   */
  Wt::WFileResource     *generateScript(long long i, Wt::WString assetName);
  std::string           getStringFromFile(std::string resourcePath);
  void                  downloadScript(std::string fileName);

  /*
   * return API after call.
   */
  void                  putAsset(boost::system::error_code err, const Wt::Http::Message& response);
  void                  postAsset(boost::system::error_code err, const Wt::Http::Message& response);
  void                  postPlugin(boost::system::error_code err, const Wt::Http::Message& response);
  void                  checkAlertsInAsset(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id);
  void                  deleteAsset(boost::system::error_code err, const Wt::Http::Message& response);
  void                  getAssets(boost::system::error_code err, const Wt::Http::Message& response);
  void                  setSession(Echoes::Dbo::Session *session);
  void                  setApiUrl(std::string apiUrl);
  
  std::string           getApiUrl();
  
  bool                          created_;
  bool                          newClass_;
  Echoes::Dbo::Session          *session_;
  std::vector<long long>        idsAlert_;
  Wt::Json::Value               result_;
  std::string                   apiUrl_;
};

#endif	/* ASSOCIATION_H */

