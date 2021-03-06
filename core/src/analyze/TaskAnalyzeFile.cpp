#include <imageanalyzer.native/core/Tasks.hpp>
#include <imageanalyzer.native/core/TaskAnalyzeExceptions.hpp>
#include <imageanalyzer.native/core/TRectangle.hpp>
#include <imageanalyzer.native/core/TMetaImage.hpp>
#include <imageanalyzer.native/core/TMetaImageJson.hpp>

#include <baseex/core/RAII.hpp>

#include <mutex>
#include <fstream>
#include <future>
#include <vector>

using namespace threadpoolex::core;

namespace imageanalyzer {
namespace native {
namespace core {

//---------------------------------------------------------------------------
class CTaskAnalyzeFile
    :public ITask, virtual CBaseObservableTask
{
public:
    CTaskAnalyzeFile(const baseex::core::CFileName &aImageName, const baseex::core::CFileName &aFileResult);

    virtual void Execute() override;

protected:
    std::vector<TRectangle> GetBlocksAnalyze(IImage::Ptr aImage, TSize aSizeAnalyze, uint8_t aX, uint8_t aY);

private:
    baseex::core::CFileName m_ImageName;
    baseex::core::CFileName m_FileResult;
};

CTaskAnalyzeFile::CTaskAnalyzeFile(const baseex::core::CFileName &aImageName, const baseex::core::CFileName &aFileResult)
    :m_ImageName(aImageName), m_FileResult(aFileResult)
{}

void CTaskAnalyzeFile::Execute()
{
    try
    {
        baseex::core::CRAII<CObservableTask::Ptr> l(this->GetObserver(), [](CObservableTask::Ptr aObserver) { aObserver->NotifyStart(); },
            [](CObservableTask::Ptr aObserver) { aObserver->NotifyComplete(); });

        TMetaImage lResult;
        {
            IImage::Ptr lImage = CreateImage(m_ImageName);
            auto lBlocks = GetBlocksAnalyze(lImage, lImage->GetSize(), 3, 3);

            for (uint32_t index = 0; index < lBlocks.size(); ++index)
                CreateTaskAnalyzeBlock(lImage, lBlocks[index], lResult.m_Histograms[index])->Execute();
        }
        std::ofstream lFileJson(m_FileResult.GetFullFileName());
        lFileJson << nlohmann::json(lResult) << std::endl;
    }
    CATCH_CODE_ERROR(baseex::core::exceptions_base::error_base, this->GetObserver()->NotifyError)
}

std::vector<TRectangle> CTaskAnalyzeFile::GetBlocksAnalyze(IImage::Ptr aImage, TSize aSizeAnalyze, uint8_t aX, uint8_t aY)
{
    std::vector<TRectangle> lResult;

    TSize lSizeBlock(aImage->GetSize().m_Width / aX, aImage->GetSize().m_Height / aY);
    for (auto iY = 0; iY < aY; ++iY)
        for (auto iX = 0; iX < aX; ++iX)
            lResult.push_back(TRectangle(TPoint(lSizeBlock.m_Width*iX, lSizeBlock.m_Height * iY), lSizeBlock));

    for (auto iY = 0; iY < aY; ++iY)
        lResult[iY*aX + aX - 1].m_Size.m_Width = aSizeAnalyze.m_Width - lResult[iY * aX + aX - 1].m_Left.m_X;

    for (auto iX = 0; iX < aX; ++iX)
        lResult[(aY - 1)*aX + iX].m_Size.m_Height = aSizeAnalyze.m_Height - lResult[(aY - 1)*aX + iX].m_Left.m_Y;

    return lResult;
}

//-------------------------------------------------------------------------

ITask::Ptr CreateTaskAnalyzeInFile(const baseex::core::CFileName &aFileName)
{
    return CreateTaskAnalyzeInFile(aFileName, baseex::core::CFileName(aFileName.GetFullFileName() + L".data"));
}
ITask::Ptr CreateTaskAnalyzeInFile(const baseex::core::CFileName &aFileName, const baseex::core::CFileName &aFileResult)
{
    return std::make_shared<CTaskAnalyzeFile>(aFileName, aFileResult);
}

}
}
}