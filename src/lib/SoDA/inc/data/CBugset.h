/*
* Copyright (C): 2015 Department of Software Engineering, University of Szeged
*
* Authors: David Havas <havasd@inf.u-szeged.hu>
*
* This file is part of SoDA.
*
*  SoDA is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  SoDA is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CBUGSET_H
#define CBUGSET_H

#include "interface/IBitList.h"
#include "interface/IIDManager.h"

namespace soda {

struct Report {
    time_t reportTime;
    time_t fixTime;
};

// code element id, bug id
typedef std::map<RevNumType, RevNumType> CodeElementReports;
typedef std::map<RevNumType, CodeElementReports> ReportMap;
typedef std::map<RevNumType, Report> ReportDataMap;

/**
* @brief The CBugset class stores which code elements were reported with an issue in multiple revisions.
*/
class CBugset {
public:

    /**
    * @brief Constructor, creates an empty CBugset object.
    */
    CBugset();

    /**
    * @brief Creates a CBugset object with specified code elements.
    * @param codeElements  Specified code elements.
    */
    CBugset(IIDManager* codeElements);

    /**
    * @brief Creates a CBugset object with specified code elements and bugset.
    * @param codeElements  Specified code elements.
    * @param revisions  Specified revision number.
    * @param reports  Bug reports data which links code elements and revisions with the appripriate bug reports.
    * @param data  Specified bug reports.
    */
    CBugset(IIDManager* codeElements, IIDManager* revisions, ReportMap* reports, ReportDataMap* data);

    /**
    * @brief Destroys a CBugset object.
    */
    virtual ~CBugset();

    /**
    * @brief Returns the reports of a specified revision number.
    * @param revisionNumber  Revision number.
    * @return Code elements and their time information fixed in the specified revision.
    */
    CodeElementReports& operator[](const String& revisionNumber) const;

    /**
    * @brief Returns the fixed code elements in a specified revision.
    * @param revisionNumber  Revision number.
    * @return Code elements and their timestamps in the specified revision.
    */
    CodeElementReports& at(const String& revisionNumber) const;

    /**
    * @brief Returns true if the specified revision exists.
    * @param revisionNumber  Specified revision.
    * @return True if the specified revision exists.
    */
    bool exists(const String& revisionNumber) const;

    /**
    * @brief Returns true if the specified code element was fixed in the specified revision.
    * @param revisionNumber  Specified revision.
    * @param codeElementName  Specified code element name.
    * @return True if the specified code element was fixed in the specified revision.
    */
    bool containsData(const String& revisionNumber, const String& codeElementName) const;

    /**
    * @brief Returns the additional report information for the specified code element and revision.
    * @param revisionNumber  Specified revision.
    * @param codeElementName  Specified code element name.
    * @return Additional report information for the specified code element and revision.
    */
    Report const& getReportInformations(String const& revisionNumber, String const& codeElementName) const;

    /**
    * @brief Returns the stored reports.
    * @return Map which stores report id and report data.
    */
    ReportDataMap const& getReports() const;

    /**
    * @brief Returns a map which stores revision, code element and report data ids.
    * @return Map which stores revisions, code elements and report id.
    */
    ReportMap const& getReportMap() const;

    /**
    * @brief Returns the code elements of a specified revision.
    * @param revisionNumber  Specified revision.
    * @return The code elements of a specified revision.
    */
    StringVector getCodeElementNames(const String& revisionNumber) const;

    /**
    * @brief Returns a reference to the code element object.
    * @return A reference to the code elements.
    */
    const IIDManager& getCodeElements() const;

    /**
    * @brief Returns revision numbers where the specified code element was affected.
    * @param codeElementName  Specified code element.
    * @return Revision numbers where the specified code element was affected.
    */
    StringVector getRevisions(const String& codeElementName) const;

    /**
    * @brief Returns the revision numbers.
    * @return Revision numbers.
    */
    StringVector getRevisions() const;

    /**
    * @brief Returns bugged code elements based on the given time.
    * @param revisionTime The designated time.
    * @return Code elements which were bugged at the given time.
    */
    virtual StringVector getBuggedCodeElements(time_t revisionTime);

    /**
    * @brief Adds a code element with a specified revision number and state.
    * @param revisionNumber  Revision number.
    * @param codeElementName  Code element name.
    * @param reportId Id of the bug report.
    * @param data  Additional information for the given code element.
    */
    virtual void addReported(const String& revisionNumber, const String& codeElementName, RevNumType const reportId, Report& data);

    /**
    * @brief Adds a code element with a specified revision number and state.
    * @param revisionNumber  Revision number.
    * @param codeElementName  Code element name.
    * @param reportId Id of the bug report.
    */
    virtual void addReported(RevNumType const revisionNumber, RevNumType const codeElementName, RevNumType const reportId);

    /**
    * @brief Adds a specified revision number to the bug set.
    * @param revisionNumber  Revision number.
    */
    virtual void addRevision(const String& revisionNumber);

    /**
    * @brief Adds multiple revision numbers to the bug set.
    * @param revisionNumbers  Specified revision numbers.
    */
    virtual void addRevision(const StringVector& revisionNumbers);

    /**
    * @brief Adds code elements from another code element IDManager.
    * @param codeElements  Code elements to be added.
    */
    virtual void addCodeElement(const IIDManager& codeElements);

    /**
    * @brief Adds multiple code elements to the bug set.
    * @param codeElements  List of code element names.
    */
    virtual void addCodeElement(const StringVector& codeElements);

    /**
    * @brief Removes the specified revision from the objecct.
    * @param revisionNumber  Specified revision number.
    */
    virtual void removeRevision(const String& revisionNumber);

    /**
    * @brief Removes the specified revisions from the object.
    * @param revisionNumbers  Multiple revision numbers.
    */
    virtual void removeRevision(const StringVector& revisionNumbers);

    /**
    * @brief Removes a specified code element from the IDManager and the bug set.
    * @param codeElementName  Specified code element.
    */
    virtual void removeCodeElement(const String& codeElementName);

    /**
    * @brief Adds a new code element to the IDManager if it's not in it.
    */
    virtual void addCodeElementName(const String&);

    /**
    * @brief Writes the content of a CBugset object to the out.
    * @param out  Output stream.
    */
    virtual void save(io::CBinaryIO* out) const;

    /**
    * @brief Writes the content of a CBugset object to a specified file.
    * @param filename  Specified file.
    */
    virtual void save(const char * filename) const;

    /**
    * @brief Writes the content of a CBugset object to a specified file.
    * @param filename  Specified file.
    */
    virtual void save(const String& filename) const;

    /**
    * @brief Loads the content of a CBugset object from in.
    * @param in  Input stream.
    */
    virtual void load(io::CSoDAio* in);

    /**
    * @brief Loads the content of a CBugset object from a specified file.
    * @param filename  Specified file.
    */
    virtual void load(const char * filename);

    /**
    * @brief Loads the content of a CBugset object from a specified file.
    * @param filename  Specified file.
    */
    virtual void load(const String& filename);

private:

    /**
    * @brief Writes the content of a revision table to the out.
    * @param out  Output stream.
    * @param chunk  Type of the data.
    */
    virtual void saveRevisionTable(io::CBinaryIO* out, const io::CSoDAio::ChunkID chunk) const;

    /**
    * @brief Loads the content of a revision table from in.
    * @param in  Input stream.
    */
    virtual void loadRevisionTable(io::CBinaryIO* in);

private:

    /**
    * @brief Stores code element id,name pairs.
    */
    IIDManager* m_codeElements;

    /**
    * @brief Maps hashed revision identifiers to ids.
    */
    IIDManager* m_revisions;

    /**
    * @brief Stores bug report ids and report datas.
    */
    ReportDataMap* m_reportDatas;

    /**
    * @brief Stores reported code elements groupped by revision and bug id.
    */
    ReportMap* m_reports;

    /**
    * @brief If true than the constructor creates m_codeElements.
    */
    bool m_createCodeElements;

    /**
    * @brief If true than the constructor creates m_reports and m_revisions.
    */
    bool m_createReports;
};

}

#endif /* CBUGSET_H */
