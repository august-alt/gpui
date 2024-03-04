/***********************************************************************************************************************
**
** Copyright (C) 2024 BaseALT Ltd. <org@basealt.ru>
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

#include "variablesmodel.h"

namespace preferences
{

VariablesModel::VariablesModel(QObject* parent)
    : QStandardItemModel(parent)
{
    this->insertColumns(0, 2);

    this->setHeaderData(0, Qt::Horizontal, QObject::tr("Variable Name"), Qt::DisplayRole);
    this->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"), Qt::DisplayRole);

    this->setItem( 0, 0, new QStandardItem("AppDataDir"));
    this->setItem( 1, 0, new QStandardItem("BinaryComputerSid"));
    this->setItem( 2, 0, new QStandardItem("BinaryUserSid"));
    this->setItem( 3, 0, new QStandardItem("CommonAppdataDir"));
    this->setItem( 4, 0, new QStandardItem("CommonDesktopDir"));
    this->setItem( 5, 0, new QStandardItem("CommonFavoritesDir"));
    this->setItem( 6, 0, new QStandardItem("CommonProgramsDir"));
    this->setItem( 7, 0, new QStandardItem("CommonStartMenuDir"));
    this->setItem( 8, 0, new QStandardItem("CommonStartUpDir"));
    this->setItem( 9, 0, new QStandardItem("ComputerName"));
    this->setItem(10, 0, new QStandardItem("CurrentProcessId"));
    this->setItem(11, 0, new QStandardItem("CurrentThreadId"));
    this->setItem(12, 0, new QStandardItem("DateTime"));
    this->setItem(13, 0, new QStandardItem("DateTimeEx"));
    this->setItem(14, 0, new QStandardItem("DesktopDir"));
    this->setItem(15, 0, new QStandardItem("DomainName"));
    this->setItem(16, 0, new QStandardItem("FavoritesDir"));
    this->setItem(17, 0, new QStandardItem("LastError"));
    this->setItem(18, 0, new QStandardItem("LastErrorText"));
    this->setItem(19, 0, new QStandardItem("LdapComputerSid"));
    this->setItem(20, 0, new QStandardItem("LdapUserSid"));
    this->setItem(21, 0, new QStandardItem("LocalTime"));
    this->setItem(22, 0, new QStandardItem("LocalTimeEx"));
    this->setItem(23, 0, new QStandardItem("LogonDomain"));
    this->setItem(24, 0, new QStandardItem("LogonServer"));
    this->setItem(25, 0, new QStandardItem("LogonUser"));
    this->setItem(26, 0, new QStandardItem("LogonUserSid"));
    this->setItem(27, 0, new QStandardItem("MacAddress"));
    this->setItem(28, 0, new QStandardItem("NetPlacesDir"));
    this->setItem(29, 0, new QStandardItem("OsVersion"));
    this->setItem(30, 0, new QStandardItem("ProgramFilesDir"));
    this->setItem(31, 0, new QStandardItem("ProgramsDir"));
    this->setItem(32, 0, new QStandardItem("RecentDocumentsDir"));
    this->setItem(33, 0, new QStandardItem("ResultCode"));
    this->setItem(34, 0, new QStandardItem("ResultText"));
    this->setItem(35, 0, new QStandardItem("ReversedComputerSid"));
    this->setItem(36, 0, new QStandardItem("ReversedUserSid"));
    this->setItem(37, 0, new QStandardItem("SendToDir"));
    this->setItem(38, 0, new QStandardItem("StartMenuDir"));
    this->setItem(39, 0, new QStandardItem("StartUpDir"));
    this->setItem(40, 0, new QStandardItem("SystemDir"));
    this->setItem(41, 0, new QStandardItem("SystemDrive"));
    this->setItem(42, 0, new QStandardItem("TempDir"));
    this->setItem(43, 0, new QStandardItem("TimeStamp"));
    this->setItem(44, 0, new QStandardItem("TraceFile"));
    this->setItem(45, 0, new QStandardItem("WindowsDir"));

    this->setItem( 0, 1, new QStandardItem(QObject::tr("The current user's Application Data directory.")));
    this->setItem( 1, 1, new QStandardItem(QObject::tr("The SID of the computer in hexadecimal format.")));
    this->setItem( 2, 1, new QStandardItem(QObject::tr("The SID of the current user in hexadecimal format. ")));
    this->setItem( 3, 1, new QStandardItem(QObject::tr("The 'all users' Application Data directory.")));
    this->setItem( 4, 1, new QStandardItem(QObject::tr("The 'all users' Desktop directory.")));
    this->setItem( 5, 1, new QStandardItem(QObject::tr("The 'all users' Explorer Favorites directory.")));
    this->setItem( 6, 1, new QStandardItem(QObject::tr("The 'all users' Programs directory.")));
    this->setItem( 7, 1, new QStandardItem(QObject::tr("The 'all users' Start Menu directory.")));
    this->setItem( 8, 1, new QStandardItem(QObject::tr("The 'all users' Startup directory.")));
    this->setItem( 9, 1, new QStandardItem(QObject::tr("The NetBIOS name of the computer.")));
    this->setItem(10, 1, new QStandardItem(QObject::tr("The numeric identity of the main client process.")));
    this->setItem(11, 1, new QStandardItem(QObject::tr("The numeric identity of the main client thread.")));
    this->setItem(12, 1, new QStandardItem(QObject::tr("The current time (UTC).")));
    this->setItem(13, 1, new QStandardItem(QObject::tr("The current time (UTC) with milliseconds.")));
    this->setItem(14, 1, new QStandardItem(QObject::tr("The current user's desktop directory.")));
    this->setItem(15, 1, new QStandardItem(QObject::tr("The domain name or workgroup of the computer.")));
    this->setItem(16, 1, new QStandardItem(QObject::tr("The current user's Explorer Favorites directory.")));
    this->setItem(17, 1, new QStandardItem(QObject::tr("The last error code encountered during configuration.")));
    this->setItem(18, 1, new QStandardItem(QObject::tr("The last error code text description.")));
    this->setItem(19, 1, new QStandardItem(QObject::tr("The SID of the computer in LDAP escaped binary format.")));
    this->setItem(20, 1, new QStandardItem(QObject::tr("The SID of the current user in LDAP escaped binary format.")));
    this->setItem(21, 1, new QStandardItem(QObject::tr("The current local time.")));
    this->setItem(22, 1, new QStandardItem(QObject::tr("The current local time with milliseconds.")));
    this->setItem(23, 1, new QStandardItem(QObject::tr("The domain of the current user.")));
    this->setItem(24, 1, new QStandardItem(QObject::tr("The domain controller that authenticated the current user.")));
    this->setItem(25, 1, new QStandardItem(QObject::tr("The user name of the current user.")));
    this->setItem(26, 1, new QStandardItem(QObject::tr("The SID of the current user.")));
    this->setItem(27, 1, new QStandardItem(QObject::tr("The first detected MAC address on the computer.")));
    this->setItem(28, 1, new QStandardItem(QObject::tr("The current user's My Network Places directory.")));
    this->setItem(29, 1, new QStandardItem(QObject::tr("The operating system: Windows Server® 2008 R2, Windows® 7, Windows Server® 2008, Windows Vista®, Windows Server 2003, Windows XP, or Unknown.")));
    this->setItem(30, 1, new QStandardItem(QObject::tr("The Windows Program Files directory.")));
    this->setItem(31, 1, new QStandardItem(QObject::tr("The current user's Programs directory.")));
    this->setItem(32, 1, new QStandardItem(QObject::tr("The current user's Recent Documents directory.")));
    this->setItem(33, 1, new QStandardItem(QObject::tr("The client's exit code.")));
    this->setItem(34, 1, new QStandardItem(QObject::tr("The client's exit code text description.")));
    this->setItem(35, 1, new QStandardItem(QObject::tr("The SID of the computer in reversed byte order hexadecimal format.")));
    this->setItem(36, 1, new QStandardItem(QObject::tr("The SID of the current user in reversed byte order hexadecimal format.")));
    this->setItem(37, 1, new QStandardItem(QObject::tr("The current user's Send to directory.")));
    this->setItem(38, 1, new QStandardItem(QObject::tr("The current user's Start Menu directory.")));
    this->setItem(39, 1, new QStandardItem(QObject::tr("The current user's Startup directory.")));
    this->setItem(40, 1, new QStandardItem(QObject::tr("The Windows system directory.")));
    this->setItem(41, 1, new QStandardItem(QObject::tr("The name of the drive from which the operating system is running.")));
    this->setItem(42, 1, new QStandardItem(QObject::tr("The current user's Temp directory as determined by Windows API.")));
    this->setItem(43, 1, new QStandardItem(QObject::tr("The time stamp of the configurations being implemented.")));
    this->setItem(44, 1, new QStandardItem(QObject::tr("The path/name of the trace file.")));
    this->setItem(45, 1, new QStandardItem(QObject::tr("The Windows directory.")));

}

VariablesModel::~VariablesModel()
{
}

}
