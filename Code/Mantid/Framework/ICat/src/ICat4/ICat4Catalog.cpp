#include "MantidAPI/CatalogFactory.h"
#include "MantidAPI/Progress.h"
#include "MantidAPI/WorkspaceFactory.h"
#include "MantidICat/ICatDOI/GSoapGenerated/ICatDOIDOIPortBindingProxy.h"
#include "MantidICat/ICat4/GSoapGenerated/ICat4ICATPortBindingProxy.h"
#include "MantidICat/ICat4/ICat4Catalog.h"
#include "MantidKernel/ConfigService.h"
#include "MantidKernel/DateAndTime.h"
#include "MantidKernel/FacilityInfo.h"
#include "MantidKernel/Logger.h"
#include "MantidKernel/Strings.h"

namespace Mantid
{
  namespace ICat
  {
    using namespace Kernel;
    using namespace ICat4;

    namespace
    {
      /// static logger
      Kernel::Logger g_log("ICat4Catalog");
    }

    DECLARE_CATALOG(ICat4Catalog)

    ICat4Catalog::ICat4Catalog() : m_session() {}

    /**
     * Authenticate the user against all catalogues in the container.
     * @param username :: The login name of the user.
     * @param password :: The password of the user.
     * @param endpoint :: The endpoint url of the catalog to log in to.
     * @param facility :: The facility of the catalog to log in to.
     */
    API::CatalogSession_sptr ICat4Catalog::login(const std::string& username,const std::string& password,
        const std::string& endpoint, const std::string& facility)
    {
      // Created the session object here in order to set the endpoint, which is used in setICATProxySettings.
      // We can then manually set the sessionID later if it exists.
      m_session = boost::make_shared<API::CatalogSession>("",facility,endpoint);

      // Securely set, including soap-endpoint.
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      // Used to authenticate the user.
      ns1__login login;
      ns1__loginResponse loginResponse;

      // Used to add entries to the login class.
      _ns1__login_credentials_entry entry;
       
      // Name of the authentication plugin in use.
      std::string plugin;

      if (endpoint.find("sns") != std::string::npos) {
        plugin = std::string("ldap");
      } else {
        plugin = std::string("uows");
      }
      login.plugin = &plugin;

      // Making string as cannot convert from const.
      std::string userName(username);
      std::string passWord(password);

      std::string usernameKey("username");
      std::string passwordKey("password");

      // Instantiate an instance of an entry to prevent null pointer.
      // This then allows us to push entries as required below.
      std::vector<_ns1__login_credentials_entry> entries;
      login.credentials.entry = &entries;

      // Setting the username and pass credentials to the login class.
      entry.key   = &usernameKey;
      entry.value = &userName;
      entries.push_back(entry);

      entry.key   = &passwordKey;
      entry.value = &passWord;
      entries.push_back(entry);

      if (icat.login(&login, &loginResponse) == SOAP_OK)
      {
        m_session->setSessionId(*(loginResponse.return_));
      }
      else
      {
        throwSoapError(icat);
      }
      // Will not reach here if user cannot log in (e.g. no session is created).
      return m_session;
    }

    /**
     * Disconnects the client application from ICat4 based catalog services.
     */
    void ICat4Catalog::logout()
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__logout request;
      ns1__logoutResponse response;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      if(icat.logout(&request,&response) == SOAP_OK)
      {
        m_session->setSessionId("");
      }
      else
      {
        throwSoapError(icat);
      }
    }

    /**
     * Creates a search query string based on inputs provided by the user.
     * @param inputs :: reference to a class contains search inputs.
     * @return a query string constructed from user input.
     */
    std::string ICat4Catalog::buildSearchQuery(const CatalogSearchParam& inputs)
    {
      // Contain the related where and join clauses for the search query based on user-input.
      std::vector<std::string> whereClause, joinClause;

      // Format the timestamps in order to compare them.
      std::string startDate = formatDateTime(inputs.getStartDate(), "%Y-%m-%d %H:%M:%S");
      std::string endDate   = formatDateTime(inputs.getEndDate() + ((23*60*60) + (59*60) + 59), "%Y-%m-%d %H:%M:%S");

      // Investigation startDate if endDate is not selected
      if (inputs.getStartDate() != 0 && inputs.getEndDate() == 0)
      {
        whereClause.push_back("inves.startDate >= '" + startDate + "'");
      }

      // Investigation endDate if startdate is not selected
      if (inputs.getEndDate() != 0 && inputs.getStartDate() == 0)
      {
        whereClause.push_back("inves.endDate <= '" + endDate + "'");
      }

      // Investigation Start and end date if both selected
      if(inputs.getStartDate() != 0 && inputs.getEndDate() != 0)
      {
        whereClause.push_back("inves.startDate BETWEEN '" + startDate + "' AND '" + endDate + "'");
      }

      // Investigation name (title)
      if(!inputs.getInvestigationName().empty())
      {
        whereClause.push_back("inves.title LIKE '%" + inputs.getInvestigationName() + "%'");
      }

      // Investigation id
      if(!inputs.getInvestigationId().empty())
      {
        whereClause.push_back("inves.name = '" + inputs.getInvestigationId() + "'");
      }

      // Investigation type
      if(!inputs.getInvestigationType().empty())
      {
        joinClause.push_back("JOIN inves.type itype");
        whereClause.push_back("itype.name = '" + inputs.getInvestigationType() + "'");
      }

      // Instrument name
      if(!inputs.getInstrument().empty())
      {
        joinClause.push_back("JOIN inves.investigationInstruments invInst");
        joinClause.push_back("JOIN invInst.instrument inst");
        whereClause.push_back("inst.fullName = '" + inputs.getInstrument() + "'");
      }

      // Keywords
      if(!inputs.getKeywords().empty())
      {
        joinClause.push_back("JOIN inves.keywords keywords");
        whereClause.push_back("keywords.name IN ('" + inputs.getKeywords() + "')");
      }

      // Sample name
      if(!inputs.getSampleName().empty())
      {
        joinClause.push_back("JOIN inves.samples sample");
        whereClause.push_back("sample.name LIKE '%" + inputs.getSampleName() + "%'");
      }

      // If the user has selected the "My data only" button.
      // (E.g. they want to display or search through all the data they have access to.
      if (inputs.getMyData())
      {
        joinClause.push_back("JOIN inves.investigationUsers users");
        joinClause.push_back("JOIN users.user user");
        whereClause.push_back("user.name = :user");
      }

      // Investigators complete name.
      if (!inputs.getInvestigatorSurName().empty())
      {
        // We join another investigationUsers & user tables as we need two aliases.
        joinClause.push_back("JOIN inves.investigationUsers usrs");
        joinClause.push_back("JOIN usrs.user usr");
        whereClause.push_back("usr.fullName LIKE '%" + inputs.getInvestigatorSurName() + "%'");
      }

      // Similar to above. We check if either has been input,
      // join the related table and add the specific WHERE clause.
      if(!inputs.getDatafileName().empty() || (inputs.getRunStart() > 0 && inputs.getRunEnd() > 0))
      {
        joinClause.push_back("JOIN inves.datasets dataset");
        joinClause.push_back("JOIN dataset.datafiles datafile");

        if (!inputs.getDatafileName().empty())
        {
          whereClause.push_back("datafile.name LIKE '%" + inputs.getDatafileName() + "%'");
        }

        if (inputs.getRunStart() > 0 && inputs.getRunEnd() > 0)
        {
          joinClause.push_back("JOIN datafile.parameters datafileparameters");
          joinClause.push_back("JOIN datafileparameters.type dtype");
          whereClause.push_back("dtype.name='run_number' AND datafileparameters.numericValue BETWEEN "
              + Strings::toString(inputs.getRunStart()) + " AND " + Strings::toString(inputs.getRunEnd()) + "");
        }
      }

      std::string query;

      // This prevents the user searching the entire archive (E.g. there is no "default" query).
      if (!whereClause.empty() || !joinClause.empty())
      {
        std::string from, join, where, orderBy, includes;

        from     = " FROM Investigation inves ";
        join     = Strings::join(joinClause.begin(), joinClause.end(), " ");
        where    = Strings::join(whereClause.begin(), whereClause.end(), " AND ");
        orderBy  = " ORDER BY inves.id DESC";
        includes = " INCLUDE inves.facility, inves.investigationInstruments.instrument, inves.parameters";

        // As we joined all WHERE clause with AND we need to include the WHERE at the start of the where segment.
        where.insert(0, " WHERE ");
        // Build the query from the result.
        query = from + join + where + orderBy + includes;
      }

      return (query);
    }

    /**
     * Searches for the relevant data based on user input.
     * @param inputs   :: reference to a class contains search inputs
     * @param outputws :: shared pointer to search results workspace
     * @param offset   :: skip this many rows and start returning rows from this point.
     * @param limit    :: limit the number of rows returned by the query.
     */
    void ICat4Catalog::search(const CatalogSearchParam& inputs, Mantid::API::ITableWorkspace_sptr& outputws,
        const int &offset, const int &limit)
    {
      std::string query = buildSearchQuery(inputs);

      // Check if the query built was valid.
      if (query.empty()) throw std::runtime_error("You have not input any terms to search for.");

      // Modify the query to include correct SELECT and LIMIT clauses.
      query.insert(0, "SELECT DISTINCT inves");
      query.append(" LIMIT " + boost::lexical_cast<std::string>(offset) + "," + boost::lexical_cast<std::string>(limit));

      g_log.debug() << "The search query in ICat4Catalog::search is: \n" << query << std::endl;

      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;
      request.query = &query;

      if (icat.search(&request, &response) == SOAP_OK)
      {
        saveInvestigations(response.return_, outputws);
      }
      else
      {
        throwSoapError(icat);
      }
    }

    /**
     * Obtain the number of investigations to be returned by the catalog.
     * @return The number of investigations returned by the search performed.
     */
    int64_t ICat4Catalog::getNumberOfSearchResults(const CatalogSearchParam& inputs)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      std::string query     = buildSearchQuery(inputs);

      if (query.empty()) throw std::runtime_error("You have not input any terms to search for.");

      query.insert(0, "SELECT COUNT(DISTINCT inves)");
      request.query         = &query;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      g_log.debug() << "The paging search query in ICat4Catalog::getNumberOfSearchResults is: \n" << query << std::endl;

      int64_t numOfResults = 0;

      if (icat.search(&request, &response) == SOAP_OK)
      {
        xsd__long * numRes = dynamic_cast<xsd__long*>(response.return_.at(0));
        numOfResults = numRes->__item;
      }
      else
      {
        throwSoapError(icat);
      }

      g_log.debug() << "The number of paging results returned in ICat4Catalog::getNumberOfSearchResults is: " << numOfResults << std::endl;

      return numOfResults;
    }

    /**
     * Returns the logged in user's investigations data.
     * @param outputws :: Pointer to table workspace that stores the data.
     */
    void ICat4Catalog::myData(Mantid::API::ITableWorkspace_sptr& outputws)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      // Prevents any calls to myData from hanging due to sending a request to icat without a session ID.
      if (m_session->getSessionId().empty()) return;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      std::string query = "SELECT DISTINCT inves "
        "FROM Investigation inves "
        "JOIN inves.investigationUsers users "
        "JOIN users.user user "
        "WHERE user.name = :user "
        "ORDER BY inves.id DESC "
        "INCLUDE inves.facility, inves.investigationInstruments.instrument, inves.parameters";
      request.query     = &query;

      if (icat.search(&request, &response) == SOAP_OK)
      {
        saveInvestigations(response.return_, outputws);
      }
      else
      {
        throwSoapError(icat);
      }
    }

    /**
     * Saves investigations to a table workspace.
     * @param response :: A vector containing the results of the search query.
     * @param outputws :: Shared pointer to output workspace.
     */
    void ICat4Catalog::saveInvestigations(std::vector<xsd__anyType*> response, API::ITableWorkspace_sptr& outputws)
    {
      if (outputws->getColumnNames().empty())
      {
        // Add rows headers to the output workspace.
        outputws->addColumn("long64","DatabaseID");
        outputws->addColumn("str","InvestigationID");
        outputws->addColumn("str","Facility");
        outputws->addColumn("str","Title");
        outputws->addColumn("str","Instrument");
        outputws->addColumn("str","Run range");
        outputws->addColumn("str","Start date");
        outputws->addColumn("str","End date");
        outputws->addColumn("str","SessionID");
      }

      // Add data to each row in the output workspace.
      std::vector<xsd__anyType*>::const_iterator iter;
      for(iter = response.begin(); iter != response.end(); ++iter)
      {
        // Cast from xsd__anyType to subclass (xsd__string).
        ns1__investigation * investigation = dynamic_cast<ns1__investigation*>(*iter);
        if (investigation)
        {
          API::TableRow table = outputws->appendRow();
          // Used to insert an empty string into the cell if value does not exist.
          std::string emptyCell("");

          // Now add the relevant investigation data to the table (They always exist).
          savetoTableWorkspace(investigation->id, table);
          savetoTableWorkspace(investigation->name, table);
          savetoTableWorkspace(investigation->facility->name, table);
          savetoTableWorkspace(investigation->title, table);
          savetoTableWorkspace(investigation->investigationInstruments.at(0)->instrument->name, table);

          // Verify that the run parameters vector exist prior to doing anything.
          // Since some investigations may not have run parameters.
          if (!investigation->parameters.empty())
          {
            savetoTableWorkspace(investigation->parameters[0]->stringValue, table);
          }
          else
          {
            savetoTableWorkspace(&emptyCell, table);
          }

          // Again, we need to check first if start and end date exist prior to insertion.
          if (investigation->startDate)
          {
            std::string startDate = formatDateTime(*investigation->startDate, "%Y-%m-%d");
            savetoTableWorkspace(&startDate, table);
          }
          else
          {
            savetoTableWorkspace(&emptyCell, table);
          }

          if (investigation->endDate)
          {
            std::string endDate = formatDateTime(*investigation->endDate, "%Y-%m-%d");
            savetoTableWorkspace(&endDate, table);
          }
          else
          {
            savetoTableWorkspace(&emptyCell, table);
          }
          std::string sessionID = m_session->getSessionId();
          savetoTableWorkspace(&sessionID, table);
        }
        else
        {
          throw std::runtime_error("ICat4Catalog::saveInvestigations expected an investigation. Please contact the Mantid development team.");
        }
      }
    }

    /**
     * Returns the datasets associated to the given investigation id.
     * @param investigationId :: unique identifier of the investigation
     * @param outputws        :: shared pointer to datasets
     */
    void ICat4Catalog::getDataSets(const std::string& investigationId, Mantid::API::ITableWorkspace_sptr& outputws)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      std::string query = "Datafile <-> Dataset <-> Investigation[name = '" + investigationId + "']";
      request.query     = &query;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      g_log.debug() << "ICat4Catalog::getDataSets -> { " << query << " }" << std::endl;

      if (icat.search(&request, &response) == SOAP_OK)
      {
        saveDataSets(response.return_, outputws);
      }
      else
      {
        throwSoapError(icat);
      }
    }

    /**
     * Loops through the response vector and saves the datasets details to a table workspace.
     * @param response :: A vector containing the results of the search query.
     * @param outputws :: Shared pointer to output workspace.
     */
    void ICat4Catalog::saveDataSets(std::vector<xsd__anyType*> response, API::ITableWorkspace_sptr& outputws)
    {
      if (outputws->getColumnNames().empty())
      {
        // Add rows headers to the output workspace.
        outputws->addColumn("str","Name");
        outputws->addColumn("str","Status");
        outputws->addColumn("str","Type");
        outputws->addColumn("str","Description");
        outputws->addColumn("str","Sample Id");
      }

      std::string temp("");

      std::vector<xsd__anyType*>::const_iterator iter;
      for(iter = response.begin(); iter != response.end(); ++iter)
      {
        API::TableRow table = outputws->appendRow();
        // These are just temporary values in order for the GUI to not die.
        // These along with related GUI aspects will be removed in another ticket.
        savetoTableWorkspace(&temp, table);
        savetoTableWorkspace(&temp, table);
        savetoTableWorkspace(&temp, table);
        savetoTableWorkspace(&temp, table);
        savetoTableWorkspace(&temp, table);
      }
    }

    /**
     * Returns the datafiles associated to the given investigation id.
     * @param investigationId  :: unique identifier of the investigation
     * @param outputws         :: shared pointer to datasets
     */
    void ICat4Catalog::getDataFiles(const std::string& investigationId, Mantid::API::ITableWorkspace_sptr& outputws)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      std::string query = "Datafile <-> Dataset <-> Investigation[name = '" + investigationId + "']";
      request.query     = &query;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      g_log.debug() << "The query for ICat4Catalog::getDataSets is:\n" << query << std::endl;

      if (icat.search(&request, &response) == SOAP_OK)
      {
        saveDataFiles(response.return_, outputws);
      }
      else
      {
        throwSoapError(icat);
      }
    }

    /**
     * Saves result from "getDataFiles" to workspace.
     * @param response :: result response from the catalog.
     * @param outputws :: shared pointer to datasets
     */
    void ICat4Catalog::saveDataFiles(std::vector<xsd__anyType*> response, API::ITableWorkspace_sptr& outputws)
    {
      if (outputws->getColumnNames().empty())
      {
        // Add rows headers to the output workspace.
        outputws->addColumn("str","Name");
        outputws->addColumn("str","Location");
        outputws->addColumn("str","Create Time");
        outputws->addColumn("long64","Id");
        outputws->addColumn("long64","File size(bytes)");
        outputws->addColumn("str","File size");
        outputws->addColumn("str","Description");
        outputws->addColumn("str","doi");
      }

      std::vector<xsd__anyType*>::const_iterator iter;
      for(iter = response.begin(); iter != response.end(); ++iter)
      {
        ns1__datafile * datafile = dynamic_cast<ns1__datafile*>(*iter);
        if (datafile)
        {
          API::TableRow table = outputws->appendRow();
          // Now add the relevant investigation data to the table.
          savetoTableWorkspace(datafile->name, table);
          savetoTableWorkspace(datafile->location, table);

          std::string createDate = formatDateTime(*datafile->createTime, "%Y-%m-%d %H:%M:%S");
          savetoTableWorkspace(&createDate, table);

          savetoTableWorkspace(datafile->id, table);
          savetoTableWorkspace(datafile->fileSize, table);

          std::string fileSize = bytesToString(*datafile->fileSize);
          savetoTableWorkspace(&fileSize, table);

          if (datafile->description) savetoTableWorkspace(datafile->description, table);
          savetoTableWorkspace(datafile->doi, table);
        }
        else
        {
          throw std::runtime_error("ICat4Catalog::saveDataFiles expected a datafile. Please contact the Mantid development team.");
        }
      }
    }

    /**
     * Returns the list of instruments.
     * @param instruments :: instruments list
     */
    void ICat4Catalog::listInstruments(std::vector<std::string>& instruments)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      std::string query = "Instrument.fullName ORDER BY fullName";
      request.query     = &query;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      if (icat.search(&request, &response) == SOAP_OK)
      {
        for(unsigned i = 0; i < response.return_.size(); ++i)
        {
          xsd__string * instrument = dynamic_cast<xsd__string*>(response.return_[i]);;
          if (instrument)
          {
            instruments.push_back(instrument->__item);
          }
          else
          {
            throw std::runtime_error("ICat4Catalog::listInstruments expected an instrument. Please contact the Mantid development team.");
          }
        }
      }
      else
      {
        throwSoapError(icat);
      }
    }

    /**
     * Returns the list of investigation types.
     * @param invstTypes :: investigation types list
     */
    void ICat4Catalog::listInvestigationTypes(std::vector<std::string>& invstTypes)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      std::string query = "InvestigationType.name ORDER BY name";
      request.query     = &query;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      if (icat.search(&request, &response) == SOAP_OK)
      {
        for(unsigned i = 0; i < response.return_.size(); ++i)
        {
          xsd__string * investigation = dynamic_cast<xsd__string*>(response.return_[i]);
          if (investigation)
          {
            invstTypes.push_back(investigation->__item);
          }
          else
          {
            throw std::runtime_error("ICat4Catalog::listInvestigationTypes expected a string. Please contact the Mantid development team.");
          }
        }
      }
      else
      {
        throwSoapError(icat);
      }
    }


    /**
     * Keep the current session alive.
     */
    void ICat4Catalog::keepAlive()
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__refresh request;
      ns1__refreshResponse response;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      // An error occurred!
      if (icat.refresh(&request,&response) != SOAP_OK) throwSoapError(icat);
    }

    /**
     * Gets the file location string from the archives.
     * @param fileID :: The id of the file to search for.
     * @return The location of the datafile stored on the archives.
     */
    const std::string ICat4Catalog::getFileLocation(const long long & fileID)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__get request;
      ns1__getResponse response;

      std::string query  = "Datafile";
      request.query      = &query;
      request.primaryKey = fileID;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      std::string fileLocation;

      if (icat.get(&request, &response) == SOAP_OK)
      {
        ns1__datafile * datafile = dynamic_cast<ns1__datafile*>(response.return_);

        if (datafile && datafile->location)
        {
          fileLocation = *(datafile->location);
        }
        else
        {
          throw std::runtime_error("ICat4Catalog::getFileLocation expected a datafile. Please contact the Mantid development team.");
        }
      }
      else
      {
        throwSoapError(icat);
      }
      return fileLocation;
    }

    /**
     * Downloads a file from the given url if not downloaded from archive.
     * @param fileID :: The id of the file to search for.
     * @return A URL to download the datafile from.
     */
    const std::string ICat4Catalog::getDownloadURL(const long long & fileID)
    {
      // Obtain the URL from the Facilities.xml file.
      std::string url = ConfigService::Instance().getFacility(m_session->getFacility()).catalogInfo().externalDownloadURL();

      // Set the REST features of the URL.
      std::string session  = "sessionId="    + m_session->getSessionId();
      std::string datafile = "&datafileIds=" + boost::lexical_cast<std::string>(fileID);
      std::string outname  = "&outname="     + boost::lexical_cast<std::string>(fileID);

      // Add all the REST pieces to the URL.
      url += ("getData?" + session + datafile + outname + "&zip=false");
      g_log.debug() << "The download URL in ICat4Catalog::getDownloadURL is: " << url << std::endl;
      return url;
    }

    /**
     * Get the URL where the datafiles will be uploaded to.
     * @param investigationID :: The investigation used to obtain the related dataset ID.
     * @param createFileName  :: The name to give to the file being saved.
     * @param dataFileDescription :: The description of the data file being saved.
     * @return URL to PUT datafiles to.
     */
    const std::string ICat4Catalog::getUploadURL(
        const std::string &investigationID, const std::string &createFileName, const std::string &dataFileDescription)
    {
      // Obtain the URL from the Facilities.xml file.
      std::string url = ConfigService::Instance().getFacility(m_session->getFacility()).catalogInfo().externalDownloadURL();

      // Set the elements of the URL.
      std::string session   = "sessionId="  + m_session->getSessionId();
      std::string name      = "&name="      + createFileName;
      std::string datasetId = "&datasetId=" + boost::lexical_cast<std::string>(getMantidDatasetId(investigationID));
      std::string description = "&description=" + dataFileDescription;

      // Add pieces of URL together.
      url += ("put?" + session + name + datasetId + description + "&datafileFormatId=1");
      g_log.debug() << "The upload URL in ICat4Catalog::getUploadURL is: " << url << std::endl;
      return url;
    }

    /**
     * Generate a DOI for a datafile based on the related investigation.
     * @param databaseID :: The database ID of the datafile's investigation to register the DOI for.
     * @return The DOI that was generated for the datafile.
     */
    const std::string ICat4Catalog::registerDatafileDOI(const long long& databaseID)
    {
      ICatDOI::DOIPortBindingProxy icatDOI;

      icatDOI.recv_timeout = boost::lexical_cast<int>(Kernel::ConfigService::Instance().getString("catalog.timeout.value"));
      icatDOI.soap_endpoint = ConfigService::Instance().getFacility(m_session->getFacility()).catalogInfo().doiEndPoint().c_str();
      setSSLContext(icatDOI);

      ICatDOI::ICatDOI1__registerDatafileDOI request;
      ICatDOI::ICatDOI1__registerDatafileDOIResponse response;

      std::string session = m_session->getSessionId();
      request.arg0 = &session;
      request.arg1 = databaseID;

      std::string registeredDOI = "";

      if (icatDOI.registerDatafileDOI(&request,&response) == SOAP_OK)
      {
        registeredDOI = *(response.return_);
      }
      else
      {
        throwSoapError(icatDOI);
      }
      return registeredDOI;
    }

    /**
    * Obtains the investigations that the user can publish
    * to and saves related information to a workspace.
    * @return A workspace containing investigation information the user can publish to.
    */
    API::ITableWorkspace_sptr ICat4Catalog::getPublishInvestigations()
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      auto ws = API::WorkspaceFactory::Instance().createTable("TableWorkspace");
      // Populate the workspace with all the investigations that
      // the user is an investigator off and has READ access to.
      myData(ws);

      ns1__isAccessAllowed request;
      ns1__isAccessAllowedResponse response;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      ns1__accessType_ acessType;
      acessType.__item = ns1__accessType__CREATE;
      request.accessType = &acessType.__item;

      // Remove each investigation returned from `myData`
      // were the user does not have create/write access.
      for (int row = static_cast<int>(ws->rowCount()) - 1; row >= 0; --row)
      {
        ns1__datafile datafile;
        std::string datafileName = "tempName.nxs";
        datafile.name = &datafileName;

        // Each investigation can have multiple datasets.
        // We want to check that the user can publish to the "mantid" dataset
        // related to the investigations of which they are investigators (via "my data").
        ns1__dataset dataset;
        int64_t datasetID = getMantidDatasetId(ws->getRef<std::string>("InvestigationID",row));
        dataset.id = &datasetID;
        datafile.dataset = &dataset;

        request.bean = &datafile;

        if (icat.isAccessAllowed(&request,&response) == SOAP_OK)
        {
          if (!response.return_) ws->removeRow(row);
        }
        else
        {
          throwSoapError(icat);
        }
      }

      return ws;
    }

    /**
     * Search the archive & obtain the "mantid" dataset ID for a specific investigation if it exists.
     * If it does not exist, we will attempt to create it.
     * @param investigationID :: Used to obtain the related dataset ID.
     * @return Dataset ID of the provided investigation.
     */
    int64_t ICat4Catalog::getMantidDatasetId(const std::string &investigationID)
    {
      ICat4::ICATPortBindingProxy icat;
      setICATProxySettings(icat);

      ns1__search request;
      ns1__searchResponse response;

      std::string query = "Dataset <-> Investigation[name = '" + investigationID + "']";
      request.query     = &query;

      std::string sessionID = m_session->getSessionId();
      request.sessionId = &sessionID;

      g_log.debug() << "The query performed to obtain a dataset from an investigation id " <<
          "in ICat4Catalog::getMantidDatasetId is: " << query << std::endl;
      
      int64_t datasetID = 0;
      
      if (icat.search(&request, &response) == SOAP_OK)
      {
        if (response.return_.size() <= 0)
        {
          throw std::runtime_error("The datafile you tried to publish has no related dataset."
              " (Based on investigation ID: " + investigationID + ")");
        }
        ns1__dataset * dataset = dynamic_cast<ns1__dataset*>(response.return_.at(0));
        if (dataset && dataset->id)
        {
          datasetID = *(dataset->id);
          g_log.debug() << "The name of the dataset related to " << investigationID << " is: " << *(dataset->name) << "\n";
        }
      }
      else
      {
        throwSoapError(icat);
      }

      return datasetID;
    }

    /**
     * Defines the SSL authentication scheme.
     * @param soapProxy :: The PortBindingProxy object.
     */
    template<class T>
    void ICat4Catalog::setSSLContext(T& soapProxy)
    {
      if (soap_ssl_client_context(&soapProxy,
          SOAP_SSL_CLIENT, /* use SOAP_SSL_DEFAULT in production code */
          NULL,       /* keyfile: required only when client must authenticate to
              server (see SSL docs on how to obtain this file) */
          NULL,       /* password to read the keyfile */
          NULL,      /* optional cacert file to store trusted certificates */
          NULL,      /* optional capath to directory with trusted certificates */
          NULL      /* if randfile!=NULL: use a file with random data to seed randomness */
      ))
      {
        throwSoapError(soapProxy);
      }
    }

    /**
     * Throws an error message (returned by gsoap) to Mantid upper layer.
     * @param soapProxy :: The PortBindingProxy object.
     */
    template<class T>
    void ICat4Catalog::throwSoapError(T& soapProxy)
    {
      std::string error = soapProxy.soap_fault_string();
      // If no error is returned by ICAT then there is a connection problem.
      if (error.empty()) error = "ICAT appears to be offline. Please check your connection or report this issue.";

      g_log.debug() << "The gSOAP error code returned by ICAT was: " << soapProxy.error << "\n";
      throw std::runtime_error(error);
    }

    /**
     * Convert a file size to human readable file format.
     * @param fileSize :: The size in bytes of the file.
     */
    std::string ICat4Catalog::bytesToString(int64_t &fileSize)
    {
      const char* args[] = {"B", "KB", "MB", "GB"};
      std::vector<std::string> units(args, args + 4);

      unsigned order = 0;

      while (fileSize >= 1024 && order + 1 < units.size())
      {
        order++;
        fileSize = fileSize / 1024;
      }

      return boost::lexical_cast<std::string>(fileSize) + units.at(order);
    }

    /**
     * Formats a given timestamp to human readable datetime.
     * @param timestamp :: Unix timestamp.
     * @param format    :: The desired format to output.
     * @return string   :: Formatted Unix timestamp.
     */
    std::string ICat4Catalog::formatDateTime(const time_t &timestamp, const std::string &format)
    {
      auto dateTime = DateAndTime(boost::posix_time::from_time_t(timestamp));
      return (dateTime.toFormattedString(format));
    }

    /**
     * Sets the soap-endpoint & SSL context for the given ICAT proxy.
     */
    void ICat4Catalog::setICATProxySettings(ICat4::ICATPortBindingProxy& icat)
    {
      // The soapEndPoint is only set when the user logs into the catalog.
      // If it's not set the correct error is returned (invalid sessionID) from the ICAT server.
      if (m_session->getSoapEndpoint().empty()) return;
      // Stop receiving packets from ICAT server after period of time.
      icat.recv_timeout = boost::lexical_cast<int>(Kernel::ConfigService::Instance().getString("catalog.timeout.value"));
      // Set the soap-endpoint of the catalog we want to use.
      icat.soap_endpoint = m_session->getSoapEndpoint().c_str();
      // Sets SSL authentication scheme
      setSSLContext(icat);
    }
  }
}
