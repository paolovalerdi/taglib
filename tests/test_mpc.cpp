#include <string>
#include <stdio.h>
#include <apetag.h>
#include <id3v1tag.h>
#include <tstringlist.h>
#include <tbytevectorlist.h>
#include <tpropertymap.h>
#include <mpcfile.h>
#include <cppunit/extensions/HelperMacros.h>
#include "utils.h"

using namespace std;
using namespace TagLib;

class TestMPC : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestMPC);
  CPPUNIT_TEST(testPropertiesSV8);
  CPPUNIT_TEST(testPropertiesSV7);
  CPPUNIT_TEST(testPropertiesSV5);
  CPPUNIT_TEST(testPropertiesSV4);
  CPPUNIT_TEST(testFuzzedFile1);
  CPPUNIT_TEST(testFuzzedFile2);
  CPPUNIT_TEST(testFuzzedFile3);
  CPPUNIT_TEST(testFuzzedFile4);
  CPPUNIT_TEST(testStripAndProperties);
  CPPUNIT_TEST_SUITE_END();

public:

  void testPropertiesSV8()
  {
    MPC::File f(TEST_FILE_PATH_C("sv8_header.mpc"));
    CPPUNIT_ASSERT(f.audioProperties());
    CPPUNIT_ASSERT_EQUAL(8, f.audioProperties()->mpcVersion());
    CPPUNIT_ASSERT_EQUAL(1, f.audioProperties()->length());
    CPPUNIT_ASSERT_EQUAL(1, f.audioProperties()->lengthInSeconds());
    CPPUNIT_ASSERT_EQUAL(1497, f.audioProperties()->lengthInMilliseconds());
    CPPUNIT_ASSERT_EQUAL(1, f.audioProperties()->bitrate());
    CPPUNIT_ASSERT_EQUAL(2, f.audioProperties()->channels());
    CPPUNIT_ASSERT_EQUAL(44100, f.audioProperties()->sampleRate());
    CPPUNIT_ASSERT_EQUAL(66014U, f.audioProperties()->sampleFrames());
  }

  void testPropertiesSV7()
  {
    MPC::File f(TEST_FILE_PATH_C("click.mpc"));
    CPPUNIT_ASSERT(f.audioProperties());
    CPPUNIT_ASSERT_EQUAL(7, f.audioProperties()->mpcVersion());
    CPPUNIT_ASSERT_EQUAL(0, f.audioProperties()->length());
    CPPUNIT_ASSERT_EQUAL(0, f.audioProperties()->lengthInSeconds());
    CPPUNIT_ASSERT_EQUAL(40, f.audioProperties()->lengthInMilliseconds());
    CPPUNIT_ASSERT_EQUAL(318, f.audioProperties()->bitrate());
    CPPUNIT_ASSERT_EQUAL(2, f.audioProperties()->channels());
    CPPUNIT_ASSERT_EQUAL(44100, f.audioProperties()->sampleRate());
    CPPUNIT_ASSERT_EQUAL(1760U, f.audioProperties()->sampleFrames());
    CPPUNIT_ASSERT_EQUAL(14221, f.audioProperties()->trackGain());
    CPPUNIT_ASSERT_EQUAL(19848, f.audioProperties()->trackPeak());
    CPPUNIT_ASSERT_EQUAL(14221, f.audioProperties()->albumGain());
    CPPUNIT_ASSERT_EQUAL(19848, f.audioProperties()->albumPeak());
  }

  void testPropertiesSV5()
  {
    MPC::File f(TEST_FILE_PATH_C("sv5_header.mpc"));
    CPPUNIT_ASSERT(f.audioProperties());
    CPPUNIT_ASSERT_EQUAL(5, f.audioProperties()->mpcVersion());
    CPPUNIT_ASSERT_EQUAL(26, f.audioProperties()->length());
    CPPUNIT_ASSERT_EQUAL(26, f.audioProperties()->lengthInSeconds());
    CPPUNIT_ASSERT_EQUAL(26371, f.audioProperties()->lengthInMilliseconds());
    CPPUNIT_ASSERT_EQUAL(0, f.audioProperties()->bitrate());
    CPPUNIT_ASSERT_EQUAL(2, f.audioProperties()->channels());
    CPPUNIT_ASSERT_EQUAL(44100, f.audioProperties()->sampleRate());
    CPPUNIT_ASSERT_EQUAL(1162944U, f.audioProperties()->sampleFrames());
  }

  void testPropertiesSV4()
  {
    MPC::File f(TEST_FILE_PATH_C("sv4_header.mpc"));
    CPPUNIT_ASSERT(f.audioProperties());
    CPPUNIT_ASSERT_EQUAL(4, f.audioProperties()->mpcVersion());
    CPPUNIT_ASSERT_EQUAL(26, f.audioProperties()->length());
    CPPUNIT_ASSERT_EQUAL(26, f.audioProperties()->lengthInSeconds());
    CPPUNIT_ASSERT_EQUAL(26371, f.audioProperties()->lengthInMilliseconds());
    CPPUNIT_ASSERT_EQUAL(0, f.audioProperties()->bitrate());
    CPPUNIT_ASSERT_EQUAL(2, f.audioProperties()->channels());
    CPPUNIT_ASSERT_EQUAL(44100, f.audioProperties()->sampleRate());
    CPPUNIT_ASSERT_EQUAL(1162944U, f.audioProperties()->sampleFrames());
  }

  void testFuzzedFile1()
  {
    MPC::File f(TEST_FILE_PATH_C("zerodiv.mpc"));
    CPPUNIT_ASSERT(f.isValid());
  }

  void testFuzzedFile2()
  {
    MPC::File f(TEST_FILE_PATH_C("infloop.mpc"));
    CPPUNIT_ASSERT(f.isValid());
  }

  void testFuzzedFile3()
  {
    MPC::File f(TEST_FILE_PATH_C("segfault.mpc"));
    CPPUNIT_ASSERT(f.isValid());
  }

  void testFuzzedFile4()
  {
    MPC::File f(TEST_FILE_PATH_C("segfault2.mpc"));
    CPPUNIT_ASSERT(f.isValid());
  }

  void testStripAndProperties()
  {
    ScopedFileCopy copy("click", ".mpc");

    {
      MPC::File f(copy.fileName().c_str());
      f.APETag(true)->setTitle("APE");
      f.ID3v1Tag(true)->setTitle("ID3v1");
      f.save();
    }
    {
      MPC::File f(copy.fileName().c_str());
      CPPUNIT_ASSERT_EQUAL(String("APE"), f.properties()["TITLE"].front());
      f.strip(MPC::File::APE);
      CPPUNIT_ASSERT_EQUAL(String("ID3v1"), f.properties()["TITLE"].front());
      f.strip(MPC::File::ID3v1);
      CPPUNIT_ASSERT(f.properties().isEmpty());
    }
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestMPC);
