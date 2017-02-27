/**
 * @file CsvUtil.h
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#ifndef CSV_UTIL_H_
#define CSV_UTIL_H_

#include <iostream>

namespace Wt {
  class WObject;
  class WAbstractItemModel;
  class WStandardItemModel;
}

extern void readFromCsv(std::istream& f, Wt::WAbstractItemModel *model,
			int numRows = -1, bool firstLineIsHeaders = true);

extern Wt::WStandardItemModel *csvToModel(const std::string& csvFile,
					  Wt::WObject *parent);

#endif // CSV_UTIL_H_
