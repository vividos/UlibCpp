//
// ulib - a collection of useful classes
// Copyright (C) 2013-2016,2017 Michael Fink
//
/// \file TestString.cpp tests CString class
//

// includes
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// Tests for class CString
   TEST_CLASS(TestString)
   {
   public:
      /// tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         CString s1, s2;
         Assert::AreEqual(s1.GetLength(), s2.GetLength());
      }

      /// tests ctors
      TEST_METHOD(TestCtors)
      {
         CString s1(_T("abc123"));
         CString s2("abc123");
         CString s3(L"abc123");
         Assert::AreEqual(s1.GetString(), s2.GetString());
         Assert::AreEqual(s1.GetString(), s3.GetString());

         CString s4(_T('a'), 3);
         CString s5(_T("aaabbb"), 3);
         Assert::AreEqual(s4.GetString(), s5.GetString());
         Assert::AreEqual(_T("aaa"), s4.GetString());

         CString s6(_T('a'));
         Assert::AreEqual(_T("a"), s6.GetString());

         CString s7(_T(""));
         LPCTSTR psz8 = nullptr;
         CString s8(psz8);
         Assert::AreEqual(s7.GetString(), s8.GetString());
      }

      /// tests converting ctors
      TEST_METHOD(TestConvertingCtors1)
      {
         CString s1(_T("abc123"));

         CStringA s2(s1);
         Assert::AreEqual("abc123", s2.GetString());

         CStringW s3(s1);
         Assert::AreEqual(L"abc123", s3.GetString());
      }

      /// tests ctors that take the "other" char type as arguments
      TEST_METHOD(TestConvertingCtors2)
      {
         CStringA s1(L"abc123");
         Assert::AreEqual("abc123", s1.GetString());

         CStringW s2("abc123");
         Assert::AreEqual(L"abc123", s2.GetString());
      }

      /// tests copy ctor
      TEST_METHOD(TestCopyCtor)
      {
         CString s1(_T("abc123"));
         CString s2(s1);

         Assert::AreEqual(_T("abc123"), s2.GetString());
      }

      /// tests GetLength()
      TEST_METHOD(TestGetLength)
      {
         CString s1(_T("abc123"));
         CString s2(_T('y'), 555);

         Assert::AreEqual(6, s1.GetLength());
         Assert::AreEqual(555, s2.GetLength());
      }

      /// tests IsEmpty()
      TEST_METHOD(TestIsEmpty)
      {
         CString s1(_T("abc123"));
         CString s2(_T(""));
         CString s3;

         Assert::IsFalse(s1.IsEmpty());
         Assert::IsTrue(s2.IsEmpty());
         Assert::IsTrue(s3.IsEmpty());
      }

      /// tests Empty()
      TEST_METHOD(TestEmpty)
      {
         CString s1(_T("abc123"));
         CString s2(_T(""));

         Assert::IsFalse(s1.IsEmpty());
         Assert::IsTrue(s2.IsEmpty());

         s1.Empty();
         s2.Empty();

         Assert::IsTrue(s1.IsEmpty());
         Assert::IsTrue(s2.IsEmpty());
      }

      /// tests GetAt()
      TEST_METHOD(TestGetAt)
      {
         CString s1(_T("abc123"));

         Assert::AreEqual(_T('b'), s1.GetAt(1));
      }

      /// tests operator[]
      TEST_METHOD(TestIndexOperator)
      {
         CString s1(_T("abc123"));

         Assert::AreEqual(_T('c'), s1[2]);
      }

      /// tests SetAt()
      TEST_METHOD(TestSetAt)
      {
         CString s1(_T("abc123"));

         s1.SetAt(2, _T('_'));
         Assert::AreEqual(_T('_'), s1.GetAt(2));
      }

      /// tests operator LPCTSTR
      TEST_METHOD(TestOperatorLPCTSTR)
      {
         CString s1(_T("abc123"));

         LPCTSTR psz1 = s1;

         Assert::AreEqual(_T("abc123"), psz1);
      }

      /// tests operator=()
      TEST_METHOD(TestAssignmentOperator)
      {
         CString s1(_T("abc123"));
         CString s2;

         s2 = s1;

         Assert::AreEqual(_T("abc123"), s2.GetString());

         s2 = s2;
         Assert::AreEqual(_T("abc123"), s2.GetString());
      }

      /// tests operator+()
      TEST_METHOD(TestAddOperator)
      {
         CString s1(_T("abc"));
         CString s2(_T("123"));

         CString s3 = s1 + s2;
         Assert::AreEqual(_T("abc123"), s3.GetString());

         s3 = s3 + s3;
         Assert::AreEqual(_T("abc123abc123"), s3.GetString());
      }

      /// tests operator+=()
      TEST_METHOD(TestInplaceAddOperator)
      {
         CString s1(_T("abc"));
         s1 += _T("123");

         Assert::AreEqual(_T("abc123"), s1.GetString());

         s1 += s1;
         Assert::AreEqual(_T("abc123abc123"), s1.GetString());
      }

      /// tests operator==()
      TEST_METHOD(TestEqualityOperator)
      {
         CString s1(_T("abc123"));
         CString s2(s1);
         CString s3(_T("123abc"));

         Assert::IsTrue(s1 == s1);
         Assert::IsTrue(s1 == s2);
         Assert::IsFalse(s1 == s3);

         Assert::IsTrue(s1 == s1.GetString());
         Assert::IsTrue(s1 == s2.GetString());
         Assert::IsFalse(s1 == s3.GetString());

         Assert::IsTrue(s1.GetString() == s1);
         Assert::IsTrue(s1.GetString() == s2);
         Assert::IsFalse(s1.GetString() == s3);
      }

      /// tests operator!=()
      TEST_METHOD(TestInequalityOperator)
      {
         CString s1(_T("abc123"));
         CString s2(s1);
         CString s3(_T("123abc"));

         Assert::IsFalse(s1 != s1);
         Assert::IsFalse(s1 != s2);
         Assert::IsTrue(s1 != s3);

         Assert::IsFalse(s1 != s1.GetString());
         Assert::IsFalse(s1 != s2.GetString());
         Assert::IsTrue(s1 != s3.GetString());

         Assert::IsFalse(s1.GetString() != s1);
         Assert::IsFalse(s1.GetString() != s2);
         Assert::IsTrue(s1.GetString() != s3);
      }

      /// tests operator<()
      TEST_METHOD(TestLessOperator)
      {
         CString s1(_T("abc"));
         CString s2(_T("def"));
         CString s3(s1);

         Assert::IsTrue(s1 < s2);
         Assert::IsFalse(s2 < s1);
         Assert::IsFalse(s1 < s3); // equal cases
         Assert::IsFalse(s3 < s1);

         Assert::IsTrue(s1 < s2.GetString());

         Assert::IsTrue(s1.GetString() < s2);
      }

      /// tests operator>()
      TEST_METHOD(TestGreaterOperator)
      {
         CString s1(_T("abc"));
         CString s2(_T("def"));
         CString s3(s1);

         Assert::IsTrue(s2 > s1);
         Assert::IsFalse(s1 > s2);
         Assert::IsFalse(s1 > s3); // equal cases
         Assert::IsFalse(s3 > s1);

         Assert::IsTrue(s2 > s1.GetString());

         Assert::IsTrue(s2.GetString() > s1);
      }

      /// tests operator<=()
      TEST_METHOD(TestLessOrEqualOperator)
      {
         CString s1(_T("abc"));
         CString s2(_T("def"));
         CString s3(s1);

         Assert::IsTrue(s1 <= s2);
         Assert::IsFalse(s2 <= s1);
         Assert::IsTrue(s1 <= s3); // equal cases
         Assert::IsTrue(s3 <= s1);

         Assert::IsTrue(s1 <= s2.GetString());

         Assert::IsTrue(s1.GetString() <= s2);
      }

      /// tests operator>=()
      TEST_METHOD(TestGreaterOrEqualOperator)
      {
         CString s1(_T("abc"));
         CString s2(_T("def"));
         CString s3(s1);

         Assert::IsTrue(s2 >= s1);
         Assert::IsFalse(s1 >= s2);
         Assert::IsTrue(s1 >= s3); // equal cases
         Assert::IsTrue(s3 >= s1);

         Assert::IsTrue(s2 >= s1.GetString());

         Assert::IsTrue(s2.GetString() >= s1);
      }

      /// tests Compare()
      TEST_METHOD(TestCompare)
      {
         CString s1(_T("abc"));
         CString s2(_T("def"));
         CString s3(s1);

         Assert::IsTrue(s1.Compare(s2) < 0);
         Assert::IsTrue(s1.Compare(s3) == 0);
         Assert::IsTrue(s2.Compare(s1) > 0);
      }

      /// tests CompareNoCase()
      TEST_METHOD(TestCompareNoCase)
      {
         CString s1(_T("abc"));
         CString s2(_T("DEF"));
         CString s3(s1);

         Assert::IsFalse(s1.Compare(s2) < 0); // strings in wrong order due to case

         Assert::IsTrue(s1.CompareNoCase(s2) < 0);
         Assert::IsTrue(s1.CompareNoCase(s3) == 0);
         Assert::IsTrue(s2.CompareNoCase(s1) > 0);
      }

      /// tests Mid()
      TEST_METHOD(TestMid)
      {
         CString s1(_T("abc123"));

         CString s2 = s1.Mid(3);
         CString s3 = s1.Mid(2, 3);
         CString s4 = s1.Mid(4, 3);
         CString s5 = s1.Mid(8, 3);
         CString s6 = s1.Mid(2, 0);

         Assert::AreEqual(_T("123"), s2.GetString());
         Assert::AreEqual(_T("c12"), s3.GetString());
         Assert::AreEqual(_T("23"), s4.GetString());
         Assert::IsTrue(s5.IsEmpty());
         Assert::IsTrue(s6.IsEmpty());
      }

      /// tests Left()
      TEST_METHOD(TestLeft)
      {
         CString s1(_T("abc123"));

         CString s2 = s1.Left(3);
         CString s3 = s1.Left(0);
         CString s4 = s1.Left(7);

         Assert::AreEqual(_T("abc"), s2.GetString());
         Assert::IsTrue(s3.IsEmpty());
         Assert::AreEqual(_T("abc123"), s4.GetString());
      }

      /// tests Right()
      TEST_METHOD(TestRight)
      {
         CString s1(_T("abc123"));

         CString s2 = s1.Right(3);
         CString s3 = s1.Right(0);
         CString s4 = s1.Right(7);

         Assert::AreEqual(_T("123"), s2.GetString());
         Assert::IsTrue(s3.IsEmpty());
         Assert::AreEqual(_T("abc123"), s4.GetString());
      }

      /// tests SpanIncluding()
      TEST_METHOD(TestSpanIncluding)
      {
         CString s1(_T("cabbage"));

         CString s2 = s1.SpanIncluding(_T("abc"));
         CString s3 = s1.SpanIncluding(_T("xyz"));

         Assert::AreEqual(_T("cabba"), s2.GetString());
         Assert::IsTrue(s3.IsEmpty());
      }

      /// tests SpanExcluding(), which returns the leftmost part of the string
      /// that doesn't contain the chars in the given set.
      TEST_METHOD(TestSpanExcluding)
      {
         CString s1(_T("cabbage"));
         CString s2 = s1.SpanExcluding(_T("bge"));
         CString s3 = s1.SpanExcluding(_T("xyz"));

         Assert::AreEqual(_T("ca"), s2.GetString());
         Assert::AreEqual(_T("cabbage"), s3.GetString());
      }

      /// tests MakeUpper()
      TEST_METHOD(TestMakeUpper)
      {
         CString s1(_T("abc123"));
         CString s2(_T("aouäöüß123"));

         s1.MakeUpper();
         s2.MakeUpper();

         Assert::AreEqual(_T("ABC123"), s1.GetString());
         //Assert::AreEqual(_T("AOUÄÖÜß123"), s2.GetString());
      }

      /// tests MakeLower()
      TEST_METHOD(TestMakeLower)
      {
         CString s1(_T("ABC123"));
         CString s2(_T("AOUÄÖÜß123"));

         s1.MakeLower();
         s2.MakeLower();

         Assert::AreEqual(_T("abc123"), s1.GetString());
         //Assert::AreEqual(_T("aouäöüß123"), s2.GetString());
      }

      /// tests MakeReverse()
      TEST_METHOD(TestMakeReverse)
      {
         CString s1(_T("abc123"));

         s1.MakeReverse();

         Assert::AreEqual(_T("321cba"), s1.GetString());
      }

      /// tests Replace()
      TEST_METHOD(TestReplace)
      {
         CString s1(_T("abc123"));
         CString s2(_T("abc123abc456"));
         CString s3(_T("aabbabacda"));

         int ret = s1.Replace(_T("bc"), _T("xy"));
         Assert::AreEqual(1, ret);
         Assert::AreEqual(_T("axy123"), s1.GetString());

         ret = s2.Replace(_T("bc"), _T("xy"));
         Assert::AreEqual(ret, 2);
         Assert::AreEqual(_T("axy123axy456"), s2.GetString());

         ret = s3.Replace(_T('a'), _T('x'));
         ret += s3.Replace(_T('b'), _T('y'));
         Assert::AreEqual(8, ret);
         Assert::AreEqual(_T("xxyyxyxcdx"), s3.GetString());
      }

      /// tests Remove()
      TEST_METHOD(TestRemove)
      {
         CString s1(_T("abc123abc456"));
         CString s2(_T("aabbabacda"));

         int ret = s1.Remove(_T('b'));
         Assert::AreEqual(2, ret);
         Assert::AreEqual(_T("ac123ac456"), s1.GetString());

         ret = s2.Remove(_T('a'));
         ret += s2.Remove(_T('b'));
         Assert::AreEqual(8, ret);
         Assert::AreEqual(_T("cd"), s2.GetString());
      }

      /// tests Insert() with strings
      TEST_METHOD(TestInsertStr)
      {
         CString s1(_T("abc123"));
         CString s2(_T("abc123"));
         CString s3(_T("abc123"));

         int ret = s1.Insert(3, _T("xyz"));
         Assert::AreEqual(ret, 9);
         Assert::AreEqual(_T("abcxyz123"), s1.GetString());

         ret = s2.Insert(6, _T("xyz"));
         Assert::AreEqual(9, ret);
         Assert::AreEqual(_T("abc123xyz"), s2.GetString());

         ret = s3.Insert(8, _T("xyz"));
         Assert::AreEqual(9, ret);
         Assert::AreEqual(_T("abc123xyz"), s3.GetString());
      }

      /// tests Insert() with chars
      TEST_METHOD(TestInsertCh)
      {
         CString s1(_T("abc123"));
         CString s2(_T("abc123"));
         CString s3(_T("abc123"));

         int ret = s1.Insert(3, _T('-'));
         Assert::AreEqual(ret, 7);
         Assert::AreEqual(_T("abc-123"), s1.GetString());

         ret = s2.Insert(6, _T('x'));
         Assert::AreEqual(ret, 7);
         Assert::AreEqual(_T("abc123x"), s2.GetString());

         ret = s3.Insert(8, _T('x'));
         Assert::AreEqual(ret, 7);
         Assert::AreEqual(_T("abc123x"), s3.GetString());
      }

      /// tests Delete()
      TEST_METHOD(TestDelete)
      {
         CString s1(_T("abc123"));
         CString s2(_T("abc123"));
         CString s3(_T("abc123"));

         int ret = s1.Delete(3);
         Assert::AreEqual(ret, 5);
         Assert::AreEqual(_T("abc23"), s1.GetString());

         ret = s2.Delete(5, 3);
         Assert::AreEqual(ret, 5);
         Assert::AreEqual(_T("abc12"), s2.GetString());

         ret = s3.Delete(7, 2);
         Assert::AreEqual(ret, 6);
         Assert::AreEqual(_T("abc123"), s3.GetString());
      }

      /// tests Format()
      TEST_METHOD(TestFormat)
      {
         CString s1;

         s1.Format(_T("%s %i %.3u % 3.4f"),
            _T("abc"), -1, 42, 64.42);

         Assert::AreEqual(_T("abc -1 042  64.4200"), s1.GetString());
      }

      static void FormatVHelper(CString& s, LPCTSTR pszText, ...)
      {
         va_list args;
         va_start(args, pszText);

         s.FormatV(pszText, args);

         va_end(args);
      }

      /// tests FormatV()
      TEST_METHOD(TestFormatV)
      {
         CString s1;

         FormatVHelper(s1, _T("%s %i %.3u % 3.4f"),
            _T("abc"), -1, 42, 64.42);

         Assert::AreEqual(_T("abc -1 042  64.4200"), s1.GetString());
      }

      /// tests AppendFormat()
      TEST_METHOD(TestAppendFormat)
      {
         CString s1(_T("abc123"));

         s1.AppendFormat(_T("%s %i %.3u % 3.4f"),
            _T("abc"), -1, 42, 64.42);

         Assert::AreEqual(_T("abc123abc -1 042  64.4200"), s1.GetString());
      }

      static void AppendFormatVHelper(CString& s, LPCTSTR pszText, ...)
      {
         va_list args;
         va_start(args, pszText);

         s.AppendFormatV(pszText, args);

         va_end(args);
      }

      /// tests AppendFormatV()
      TEST_METHOD(TestAppendFormatV)
      {
         CString s1(_T("abc123"));

         AppendFormatVHelper(s1, _T("%s %i %.3u % 3.4f"),
            _T("abc"), -1, 42, 64.42);

         Assert::AreEqual(_T("abc123abc -1 042  64.4200"), s1.GetString());
      }

      /// tests TrimLeft()
      TEST_METHOD(TestTrimLeft)
      {
         CString s1(_T("  \t\nabc123 \n\t"));
         CString s2(_T("abc123"));
         CString s3(_T("aaabc123333"));

         s1.TrimLeft();
         Assert::AreEqual(_T("abc123 \n\t"), s1.GetString());

         s2.TrimLeft(_T("abc"));
         Assert::AreEqual(_T("123"), s2.GetString());

         s3.TrimLeft(_T('a'));
         Assert::AreEqual(_T("bc123333"), s3.GetString());
      }

      /// tests TrimRight()
      TEST_METHOD(TestTrimRight)
      {
         CString s1(_T("  \t\nabc123 \n\t"));
         CString s2(_T("abc123"));
         CString s3(_T("aaabc123333"));

         s1.TrimRight();
         Assert::AreEqual(_T("  \t\nabc123"), s1.GetString());

         s2.TrimRight(_T("123"));
         Assert::AreEqual(_T("abc"), s2.GetString());

         s3.TrimRight(_T('3'));
         Assert::AreEqual(_T("aaabc12"), s3.GetString());
      }

      /// tests Trim()
      TEST_METHOD(TestTrim)
      {
         CString s1(_T("  \t\nabc123 \n\t"));
         CString s2(_T("abc123"));
         CString s3(_T("aaabc123333"));

         s1.Trim();
         Assert::AreEqual(_T("abc123"), s1.GetString());

         s2.Trim(_T("ab23"));
         Assert::AreEqual(_T("c1"), s2.GetString());

         s3.Trim(_T('3'));
         Assert::AreEqual(_T("aaabc12"), s3.GetString());
      }

      /// tests Find()
      TEST_METHOD(TestFind)
      {
         CString s1(_T("abc123"));
         CString s2(_T("abc123abc"));

         Assert::AreEqual(2, s1.Find(_T("c1")));
         Assert::AreEqual(-1, s1.Find(_T("c1"), 3));

         Assert::AreEqual(3, s1.Find(_T('1')));
         Assert::AreEqual(-1, s1.Find(_T('1'), 4));

         Assert::AreEqual(6, s2.Find(_T("a"), 2));
         Assert::AreEqual(6, s2.Find(_T('a'), 2));
      }

      /// tests ReverseFind()
      TEST_METHOD(TestReverseFind)
      {
         CString s1(_T("123abc123"));

         Assert::AreEqual(6, s1.ReverseFind(_T('1')));
         Assert::AreEqual(-1, s1.ReverseFind(_T('x')));
      }

      /// tests FindOneOf()
      TEST_METHOD(TestFindOneOf)
      {
         CString s1(_T("abc123"));

         Assert::AreEqual(3, s1.FindOneOf(_T("dx1")));
         Assert::AreEqual(2, s1.FindOneOf(_T("3cx")));
         Assert::AreEqual(-1, s1.FindOneOf(_T("xyz")));
      }

      /// tests GetBuffer(), ReleaseBuffer()
      TEST_METHOD(TestGetBufferReleaseBuffer)
      {
         CString s1(_T("abc123"));

         LPTSTR psz1 = s1.GetBuffer(7);
         Assert::AreEqual(_T("abc123"), psz1);

         psz1[2] = _T('x');

         s1.ReleaseBuffer();
         Assert::AreEqual(_T("abx123"), s1.GetString());
      }

      /// tests GetBuffer(), ReleaseBufferSetLength()
      TEST_METHOD(TestGetBufferReleaseBufferSetLength)
      {
         CString s1(_T("abc123"));

         LPTSTR psz1 = s1.GetBuffer();
         Assert::AreEqual(_T("abc123"), psz1);

         psz1[2] = _T('y');

         s1.ReleaseBufferSetLength(4);
         Assert::AreEqual(_T("aby1"), s1.GetString());
      }

      // missing tests from CStringT: Tokenize
      // missing tests from CSimpleStringT: GetBufferSetLength, FreeExtra, LockBuffer, UnlockBuffer

   }; // class TestString

} // namespace UnitTest
