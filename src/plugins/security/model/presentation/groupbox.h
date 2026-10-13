/***********************************************************************************************************************
 **
 ** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
 **
 ** This program is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU General Public License
 ** as published by the Free Software Foundation; either version 2
 ** of the License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 **
 ***********************************************************************************************************************/

#ifndef SECURITY_GROUPBOX_H
#define SECURITY_GROUPBOX_H

#include "dataelementcontent.h"
#include "presentationelements.h"

#include <memory>

namespace security
{

class PresentationElements;

/*!
 * \class GroupBox
 * \brief The GroupBox class are used to provide an identifiable grouping for other graphical elements.
 *
 * \ingroup security
 * \ingroup presentation
 */
class GroupBox: public DataElementContent
{
public:

    /*!
     * \brief hasCheckBox If group box has checkbox.
     */
    bool hasCheckBox { false };

    /*!
     * \brief defaultChecked If group box's check box is checked.
     */
    bool defaultChecked { false };

    /*!
     * \brief elements Content of the group box.
     */
    std::unique_ptr<PresentationElements> elements{};

    /*!
     * \brief label Description of the group box.
     */
    std::string label{};

    /*!
     * \brief hasBorder If group box is flat or it has border.
     */
    bool hasBorder { false };
};

} // of namespace security

#endif // SECURITY_GROUPBOX_H
