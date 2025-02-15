/*
   Copyright (c) 2018, MariaDB Corporation.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */


#ifndef LEX_STRING_INCLUDED
#define LEX_STRING_INCLUDED


typedef struct st_mysql_const_lex_string LEX_CSTRING;


class Lex_cstring : public LEX_CSTRING
{
  public:
  Lex_cstring()
  {
    str= NULL;
    length= 0;
  }
  Lex_cstring(const LEX_CSTRING &str)
  {
    LEX_CSTRING::operator=(str);
  }
  Lex_cstring(const char *_str, size_t _len)
  {
    str= _str;
    length= _len;
  }
  Lex_cstring(const char *start, const char *end)
  {
    DBUG_ASSERT(start <= end);
    str= start;
    length= end - start;
  }
  void set(const char *_str, size_t _len)
  {
    str= _str;
    length= _len;
  }
  /*
    Return the "n" leftmost bytes if this[0] is longer than "n" bytes,
    or return this[0] itself otherwise.
  */
  Lex_cstring left(size_t n) const
  {
    return Lex_cstring(str, MY_MIN(length, n));
  }
  /*
    If this[0] is shorter than "pos" bytes, then return an empty string.
    Otherwise, return a substring of this[0] starting from
    the byte position "pos" until the end.
  */
  Lex_cstring substr(size_t pos) const
  {
    return length <= pos ? Lex_cstring(str + length, (size_t) 0) :
                           Lex_cstring(str + pos, length - pos);
  }
  // Check if a prefix of this[0] is equal to "rhs".
  bool starts_with(const LEX_CSTRING &rhs) const
  {
    DBUG_ASSERT(str);
    DBUG_ASSERT(rhs.str);
    return length >= rhs.length && !memcmp(str, rhs.str, rhs.length);
  }
};


class Lex_cstring_strlen: public Lex_cstring
{
public:
  Lex_cstring_strlen(const char *from)
   :Lex_cstring(from, from ? strlen(from) : 0)
  { }
};


/* Functions to compare if two lex strings are equal */

static inline bool lex_string_cmp(CHARSET_INFO *charset, const LEX_CSTRING *a,
                                  const LEX_CSTRING *b)
{
  return my_strcasecmp(charset, a->str, b->str);
}

/*
  Compare to LEX_CSTRING's and return 0 if equal
*/

static inline bool cmp(const LEX_CSTRING *a, const LEX_CSTRING *b)
{
  return a->length != b->length ||
    (a->length && memcmp(a->str, b->str, a->length));
}
static inline bool cmp(const LEX_CSTRING a, const LEX_CSTRING b)
{
  return a.length != b.length || (a.length && memcmp(a.str, b.str, a.length));
}

/*
  Compare if two LEX_CSTRING are equal. Assumption is that
  character set is ASCII (like for plugin names)
*/

static inline bool lex_string_eq(const LEX_CSTRING *a, const LEX_CSTRING *b)
{
  if (a->length != b->length)
    return 0;                                   /* Different */
  return strcasecmp(a->str, b->str) == 0;
}

/*
  To be used when calling lex_string_eq with STRING_WITH_LEN() as second
  argument
*/

static inline bool lex_string_eq(const LEX_CSTRING *a, const char *b, size_t b_length)
{
  if (a->length != b_length)
    return 0;                                   /* Different */
  return strcasecmp(a->str, b) == 0;
}

#endif /* LEX_STRING_INCLUDED */
