/* 
 * File:   EscapeOStream.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:15
 */

#ifndef ESCAPEOSTREAM_H
#define	ESCAPEOSTREAM_H

#include <Wt/WStringStream>

namespace Wt {

class WT_API EscapeOStream
{
public:
  enum RuleSet { Empty = 0, HtmlAttribute = 1,
		 JsStringLiteralSQuote = 2, JsStringLiteralDQuote = 3, 
                 PlainText = 4, PlainTextNewLines = 5 };

  EscapeOStream();
  EscapeOStream(std::ostream& sink);
  EscapeOStream(EscapeOStream& other);

  void pushEscape(RuleSet rules);
  void popEscape();

#ifdef WT_TARGET_JAVA
  EscapeOStream& push();
#endif // WT_TARGET_JAVA

  void append(const std::string& s, const EscapeOStream& rules);

  EscapeOStream& operator<< (char);
  EscapeOStream& operator<< (const char *s);
  EscapeOStream& operator<< (const std::string& s);
  EscapeOStream& operator<< (int);
  EscapeOStream& operator<< (const EscapeOStream& other);

  const char *c_str(); // for default constructor, can return 0
  std::string str() const; // for default constructor

  bool empty() const;
  void clear();

private:
  WStringStream stream_;

  struct Entry {
    char c;
    std::string s;
  };
  std::vector<Entry> mixed_;
  std::string special_;
  const char *c_special_;

  void mixRules();
  void put(const char *s, const EscapeOStream& rules);

  void sAppend(char c);
  void sAppend(const char *s, int length);
  void sAppend(const std::string& s);

  std::vector<RuleSet> ruleSets_;

  static const std::vector<Entry> standardSets_[6];
  static const std::string standardSetsSpecial_[6];

  static const Entry htmlAttributeEntries_[3];
  static const Entry jsStringLiteralSQuoteEntries_[5];
  static const Entry jsStringLiteralDQuoteEntries_[5];
  static const Entry plainTextEntries_[3];
  static const Entry plainTextNewLinesEntries_[4];
};

}


#endif	/* ESCAPEOSTREAM_H */

