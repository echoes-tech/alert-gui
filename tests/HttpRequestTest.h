/*
 * File:   HttpRequestTest.h
 * Author: vpl
 *
 * Created on 31 déc. 2014, 11:15:13
 */

#ifndef HTTPREQUESTTEST_H
#define	HTTPREQUESTTEST_H

#include <cppunit/extensions/HelperMacros.h>

class HttpRequestTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(HttpRequestTest);

    CPPUNIT_TEST(testSetApiUrl);
    CPPUNIT_TEST(testSetLogin);
    CPPUNIT_TEST(testSetToken);
    CPPUNIT_TEST(testSetXmlPageName);
    CPPUNIT_TEST(testGetApiUrl);
    CPPUNIT_TEST(testGetLogin);
    CPPUNIT_TEST(testGetToken);
    CPPUNIT_TEST(testGetXmlPageName);
    CPPUNIT_TEST(testGetApiAddress);
    CPPUNIT_TEST(testHttpRequest);
    CPPUNIT_TEST(testHttpRequest2);
    CPPUNIT_TEST(functorJsonValue);
    CPPUNIT_TEST(testSendGet);

    CPPUNIT_TEST_SUITE_END();

public:
    friend class HttpRequest;
    HttpRequestTest();
    virtual ~HttpRequestTest();
    void setUp();
    void tearDown();

private:
    void testHttpRequest();
    void testHttpRequest2();
    void testGetApiAddress();
    void testGetApiUrl();
    void testGetLogin();
    void testGetToken();
    void testGetXmlPageName();
    void testSendGet();
    void testSetApiUrl();
    void testSetLogin();
    void testSetToken();
    void testSetXmlPageName();
    void functorJsonValue(Wt::Json::Value value);

};

#endif	/* HTTPREQUESTTEST_H */

