// -*- C++ -*-
//
// Package:    Simple_analyzer/analyzer
// Class:      analyzer
//
/**\class analyzer analyzer.cc Simple_analyzer/analyzer/plugins/analyzer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Mauricio Thiel
//         Created:  Tue, 24 Nov 2020 18:50:20 GMT
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"

#include "DataFormats/RPCRecHit/interface/RPCCSCSegmentCollection.h"


#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHit.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCChamber.h"
#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"

#include "RecoLocalMuon/RPCRecHit/src/CSCStationIndex.h"
#include "RecoLocalMuon/RPCRecHit/src/CSCObjectMap.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include "Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

#include <string>

class analyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
	public:
		explicit analyzer(const edm::ParameterSet&);
		~analyzer();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


	private:
		virtual void beginJob() override;
		virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
		virtual void endJob() override;

		// ----------member data ---------------------------
		edm::EDGetTokenT<CSCSegmentCollection> cscseg_token;
		edm::EDGetTokenT<RPCCSCSegmentCollection> rpccscseg_token;

                edm::Handle<CSCSegmentCollection> CSCSegments;
                edm::Handle<RPCCSCSegmentCollection> RPCCSCSegments;

		TH1D *h1_cscseg_rpccscseg;


		const char *label_v[320] = {"ME+1/1/1","ME+1/4/1","ME+1/1/3","ME+1/4/3","ME+1/1/5","ME+1/4/5","ME+1/1/7","ME+1/4/7","ME+1/1/9","ME+1/4/9","ME+1/1/11","ME+1/4/11","ME+1/1/13","ME+1/4/13","ME+1/1/15","ME+1/4/15","ME+1/1/17","ME+1/4/17","ME+1/1/19","ME+1/4/19","ME+1/1/21","ME+1/4/21","ME+1/1/23","ME+1/4/23","ME+1/1/25","ME+1/4/25","ME+1/1/27","ME+1/4/27","ME+1/1/29","ME+1/4/29","ME+1/1/31","ME+1/4/31","ME+1/1/33","ME+1/4/33","ME+1/1/35","ME+1/4/35","ME+1/1/2","ME+1/4/2","ME+1/1/4","ME+1/4/4","ME+1/1/6","ME+1/4/6","ME+1/1/8","ME+1/4/8","ME+1/1/10","ME+1/4/10","ME+1/1/12","ME+1/4/12","ME+1/1/14","ME+1/4/14","ME+1/1/16","ME+1/4/16","ME+1/1/18","ME+1/4/18","ME+1/1/20","ME+1/4/20","ME+1/1/22","ME+1/4/22","ME+1/1/24","ME+1/4/24","ME+1/1/26","ME+1/4/26","ME+1/1/28","ME+1/4/28","ME+1/1/30","ME+1/4/30","ME+1/1/32","ME+1/4/32","ME+1/1/34","ME+1/4/34","ME+1/1/36","ME+1/4/36","ME+1/2/1","ME+1/2/3","ME+1/2/5","ME+1/2/7","ME+1/2/9","ME+1/2/11","ME+1/2/13","ME+1/2/15","ME+1/2/17","ME+1/2/19","ME+1/2/21","ME+1/2/23","ME+1/2/25","ME+1/2/27","ME+1/2/29","ME+1/2/31","ME+1/2/33","ME+1/2/35","ME+1/2/2","ME+1/2/4","ME+1/2/6","ME+1/2/8","ME+1/2/10","ME+1/2/12","ME+1/2/14","ME+1/2/16","ME+1/2/18","ME+1/2/20","ME+1/2/22","ME+1/2/24","ME+1/2/26","ME+1/2/28","ME+1/2/30","ME+1/2/32","ME+1/2/34","ME+1/2/36","ME+1/3/1","ME+1/3/2","ME+1/3/3","ME+1/3/4","ME+1/3/5","ME+1/3/6","ME+1/3/7","ME+1/3/8","ME+1/3/9","ME+1/3/10","ME+1/3/11","ME+1/3/12","ME+1/3/13","ME+1/3/14","ME+1/3/15","ME+1/3/16","ME+1/3/17","ME+1/3/18","ME+1/3/19","ME+1/3/20","ME+1/3/21","ME+1/3/22","ME+1/3/23","ME+1/3/24","ME+1/3/25","ME+1/3/26","ME+1/3/27","ME+1/3/28","ME+1/3/29","ME+1/3/30","ME+1/3/31","ME+1/3/32","ME+1/3/33","ME+1/3/34","ME+1/3/35","ME+1/3/36","ME+2/1/1","ME+2/1/3","ME+2/1/5","ME+2/1/7","ME+2/1/9","ME+2/1/11","ME+2/1/13","ME+2/1/15","ME+2/1/17","ME+2/1/2","ME+2/1/4","ME+2/1/6","ME+2/1/8","ME+2/1/10","ME+2/1/12","ME+2/1/14","ME+2/1/16","ME+2/1/18","ME+2/2/1","ME+2/2/3","ME+2/2/5","ME+2/2/7","ME+2/2/9","ME+2/2/11","ME+2/2/13","ME+2/2/15","ME+2/2/17","ME+2/2/19","ME+2/2/21","ME+2/2/23","ME+2/2/25","ME+2/2/27","ME+2/2/29","ME+2/2/31","ME+2/2/33","ME+2/2/35","ME+2/2/2","ME+2/2/4","ME+2/2/6","ME+2/2/8","ME+2/2/10","ME+2/2/12","ME+2/2/14","ME+2/2/16","ME+2/2/18","ME+2/2/20","ME+2/2/22","ME+2/2/24","ME+2/2/26","ME+2/2/28","ME+2/2/30","ME+2/2/32","ME+2/2/34","ME+2/2/36","ME+3/1/1","ME+3/1/3","ME+3/1/5","ME+3/1/7","ME+3/1/9","ME+3/1/11","ME+3/1/13","ME+3/1/15","ME+3/1/17","ME+3/1/2","ME+3/1/4","ME+3/1/6","ME+3/1/8","ME+3/1/10","ME+3/1/12","ME+3/1/14","ME+3/1/16","ME+3/1/18","ME+3/2/1","ME+3/2/3","ME+3/2/5","ME+3/2/7","ME+3/2/9","ME+3/2/11","ME+3/2/13","ME+3/2/15","ME+3/2/17","ME+3/2/19","ME+3/2/21","ME+3/2/23","ME+3/2/25","ME+3/2/27","ME+3/2/29","ME+3/2/31","ME+3/2/33","ME+3/2/35","ME+3/2/2","ME+3/2/4","ME+3/2/6","ME+3/2/8","ME+3/2/10","ME+3/2/12","ME+3/2/14","ME+3/2/16","ME+3/2/18","ME+3/2/20","ME+3/2/22","ME+3/2/24","ME+3/2/26","ME+3/2/28","ME+3/2/30","ME+3/2/32","ME+3/2/34","ME+3/2/36","ME+4/1/1","ME+4/1/3","ME+4/1/5","ME+4/1/7","ME+4/1/9","ME+4/1/11","ME+4/1/13","ME+4/1/15","ME+4/1/17","ME+4/1/2","ME+4/1/4","ME+4/1/6","ME+4/1/8","ME+4/1/10","ME+4/1/12","ME+4/1/14","ME+4/1/16","ME+4/1/18","ME+4/2/1","ME+4/2/3","ME+4/2/5","ME+4/2/7","ME+4/2/9","ME+4/2/11","ME+4/2/13","ME+4/2/15","ME+4/2/17","ME+4/2/19","ME+4/2/21","ME+4/2/23","ME+4/2/25","ME+4/2/27","ME+4/2/29","ME+4/2/31","ME+4/2/33","ME+4/2/35","ME+4/2/2","ME+4/2/4","ME+4/2/6","ME+4/2/8","ME+4/2/10","ME+4/2/12","ME+4/2/14","ME+4/2/16","ME+4/2/18","ME+4/2/20","ME+4/2/22","ME+4/2/24","ME+4/2/26","ME+4/2/28","ME+4/2/30","ME+4/2/32","ME+4/2/34","ME+4/2/36","ME-1/1/1","ME-1/4/1","ME-1/1/3","ME-1/4/3","ME-1/1/5","ME-1/4/5","ME-1/1/7","ME-1/4/7","ME-1/1/9","ME-1/4/9","ME-1/1/11","ME-1/4/11","ME-1/1/13"};



};

analyzer::analyzer(const edm::ParameterSet& iConfig):
	cscseg_token(consumes<CSCSegmentCollection>(iConfig.getParameter<edm::InputTag>("cscSegTag")))
	, rpccscseg_token(consumes<RPCCSCSegmentCollection>(iConfig.getParameter<edm::InputTag>("rpccscSegTag")))
{

	usesResource("TFileService");
	edm::Service<TFileService> fs;


	h1_cscseg_rpccscseg = fs->make<TH1D>("h1_cscseg_rpccscseg"," ;CSC Chamber; ", 320, 0, 320);
	for (int ii=1;ii<=320;ii++) h1_cscseg_rpccscseg->GetXaxis()->SetBinLabel(ii,label_v[ii-1]);

}


analyzer::~analyzer()
{

}

void analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	edm::ESHandle<CSCGeometry> cscGeo;
	iSetup.get<MuonGeometryRecord>().get(cscGeo);

	iEvent.getByToken(cscseg_token, CSCSegments);
	CSCSegmentCollection::const_iterator cscSegment;

        iEvent.getByToken(rpccscseg_token, RPCCSCSegments);
        RPCCSCSegmentCollection::const_iterator rpccscSegment;

        std::cout << std::endl;
	for(cscSegment = CSCSegments->begin(); cscSegment != CSCSegments->end(); cscSegment++){
		CSCDetId cscId = cscSegment->cscDetId();
		for (int iii=1;iii<=319;iii++){ 
			if(strcmp(cscId.chamberName().c_str(),label_v[iii-1])==0){ 
				std::cout << "cscId.chamberName().c_str(): " << cscId.chamberName().c_str() << "; label_v[iii-1]: " << label_v[iii-1] << "   ----->>" << iii-1 << std::endl;
				h1_cscseg_rpccscseg->Fill(iii-1);
			}
		}
	}
	std::cout << "========================================================" << std::endl;

        for(rpccscSegment = RPCCSCSegments->begin(); rpccscSegment != RPCCSCSegments->end(); rpccscSegment++){
                CSCDetId cscId = rpccscSegment->cscDetId();
                for (int iii=1;iii<=319;iii++){ 
                        if(strcmp(cscId.chamberName().c_str(),label_v[iii-1])==0){ 
                                std::cout << "cscId.chamberName().c_str(): " << cscId.chamberName().c_str() << "; label_v[iii-1]: " << label_v[iii-1] << "   ----->>" << iii-1 << std::endl;
                        }
                }
        }



#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
	ESHandle<SetupData> pSetup;
	iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void analyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void analyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void analyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);

	//Specify that only 'tracks' is allowed
	//To use, remove the default given above and uncomment below
	//ParameterSetDescription desc;
	//desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
	//descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(analyzer);
