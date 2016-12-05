#ifndef PandaProd_Producer_JetsFiller_h
#define PandaProd_Producer_JetsFiller_h

#include "FillerBase.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

class JetCorrectionUncertainty;

class JetsFiller : public FillerBase {
 public:
  JetsFiller(std::string const&, edm::ParameterSet const&, edm::ConsumesCollector&);
  ~JetsFiller();

  void branchNames(panda::utils::BranchList& eventBranches, panda::utils::BranchList&) const override;
  void fill(panda::Event&, edm::Event const&, edm::EventSetup const&) override;

 protected:
  virtual void fillDetails_(panda::Event&, edm::Event const&, edm::EventSetup const&) {}

  enum OutputType {
    kCHSAK4,
    kPuppiAK4,
    kCHSAK8,
    kPuppiAK8,
    kCHSCA15,
    kPuppiCA15
  };

  typedef edm::View<reco::Jet> JetView;

  NamedToken<JetView> jetsToken_;
  NamedToken<reco::GenJetCollection> genJetsToken_;
  NamedToken<double> rhoToken_;

  JetCorrectionUncertainty* jecUncertainty_{0};

  OutputType outputType_{kCHSAK4};

  //! jet radius
  double R_{0.4};
  double minPt_{15.};
  double maxEta_{4.7};
};

#endif