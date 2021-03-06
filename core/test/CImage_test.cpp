#include <imageanalyzer.native/core/IImage.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace imageanalyzer::native::core;
using namespace baseex::core;

class CImage_test
    :public ::testing::Test
{
public:
    void SetUp()
    {}
};

TEST_F(CImage_test, GetSize_Width_3x3)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_1.png"));
    ASSERT_EQ(3, lImage->GetSize().m_Width);
}

TEST_F(CImage_test, GetSize_Height_3x3)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_1.png"));
    ASSERT_EQ(3, lImage->GetSize().m_Height);
}

TEST_F(CImage_test, GetSize_Width_1x3)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_2.png"));
    ASSERT_EQ(1, lImage->GetSize().m_Width);
}

TEST_F(CImage_test, GetSize_Height_1x3)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_2.png"));
    ASSERT_EQ(3, lImage->GetSize().m_Height);
}

TEST_F(CImage_test, GetSize_Width_3x1)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_3.png"));
    ASSERT_EQ(3, lImage->GetSize().m_Width);
}

TEST_F(CImage_test, GetSize_Height_3x1)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_3.png"));
    ASSERT_EQ(1, lImage->GetSize().m_Height);
}

TEST_F(CImage_test, GetColor_Black)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_1.png"));
    ASSERT_EQ(TColor(0, 0, 0), lImage->GetColor({ 0,0 }));
}

TEST_F(CImage_test, GetColor_White)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_1.png"));
    ASSERT_EQ(TColor(255, 255, 255), lImage->GetColor({ 1,0 }));
}

TEST_F(CImage_test, GetColors_Black)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_1.png"));
    ASSERT_EQ(TColor(0, 0, 0), TColor::FromRGB(lImage->GetColors({{ 0,0 }, {1,1}})->GetElement<uint32_t>(0)));
}

TEST_F(CImage_test, GetColors_White)
{
    IImage::Ptr lImage = CreateImage(CFileName(L"../CImage_test_1.png"));
    ASSERT_EQ(TColor(255, 255, 255), TColor::FromRGB(lImage->GetColors({{ 1,0 }, { 1,1 }})->GetElement<uint32_t>(0)));
}
