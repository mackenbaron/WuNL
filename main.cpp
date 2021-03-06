#include <iostream>
#include "rtpReceiver.h"
#include "channel.h"
#include "fmDecoder.h"
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include "fmDecoder.h"
#include "glViewer.h"
#include <queue>


typedef std::vector<boost::shared_ptr<rtpReceiver> > rtpRecvPtrVec;
typedef std::vector<boost::shared_ptr<fmDecoder> > fmDecoderPtrVec;
typedef std::vector<boost::shared_ptr<int> > vs;
int main()
{
    boost::shared_ptr<std::vector<channel> >  channelVecPtr = boost::make_shared<std::vector<channel> >(CHANNELNUM);
    rtpRecvPtrVec v(CHANNELNUM);
    fmDecoderPtrVec fv(CHANNELNUM);
    boost::shared_ptr<std::vector<int> > readIndex = boost::make_shared<std::vector<int> >(CHANNELNUM,0);
    boost::shared_ptr<std::vector<int> > writeIndex = boost::make_shared<std::vector<int> >(CHANNELNUM,0);
    std::vector<std::vector<AVFrame*> > pFrameVec(CHANNELNUM,std::vector<AVFrame*>(FRAME_BUFFER, av_frame_alloc()));
    std::vector<std::queue<AVFrame*> > pFrameQueueVec(CHANNELNUM);
    boost::shared_ptr<std::vector<std::queue<AVFrame*> > > pFrameQueueVecPtr = boost::make_shared<std::vector<std::queue<AVFrame*> > >(pFrameQueueVec);
    boost::shared_ptr<std::vector<std::vector<AVFrame*> > > pFrameVecPtr = boost::make_shared<std::vector<std::vector<AVFrame*> > >(pFrameVec);
    printf("%d\n",pFrameVecPtr->size());
    pFrameVec[0][1];
    for(std::vector<boost::shared_ptr<rtpReceiver> >::iterator pos =v.begin(); pos!=v.end(); ++pos)
    {
        int position = (pos)-v.begin();
        (*pos) = boost::make_shared<rtpReceiver> ("127.0.0.1",8080+position,position);
        (*pos)->setChannelVecPtr(channelVecPtr);
        (*pos)->start();
    }
    for(std::vector<boost::shared_ptr<fmDecoder> >::iterator pos =fv.begin(); pos!=fv.end(); ++pos)
    {
        //std::vector<boost::shared_ptr<fmDecoder> >::iterator pos =fv.begin();
        int position = (pos)-fv.begin();
        (*pos) = boost::make_shared<fmDecoder> ();
        (*pos)->setPtr(channelVecPtr,pFrameVecPtr,readIndex,writeIndex);
        (*pos)->setQueuePtr(pFrameQueueVecPtr);
        (*pos)->setThreadSeq(position);
        (*pos)->SetScreanNum(4);
        (*pos)->startDecode();
    }
    glViewer gv;
    gv.setQueuePtr(pFrameQueueVecPtr);
    gv.init();
    getchar();
    return 0;
}

