#ifndef NTUPLEFILLERBDT_H
#define NTUPLEFILLERBDT_H

#include "tthAnalysis/HiggsToTauTau/interface/NtupleFillerUnits.h" // BasicFiller<>, TTree,
 // std::is_arithmetic<>, std::enable_if<>, std::string, std::map<>
#include "tthAnalysis/HiggsToTauTau/interface/HistManagerBase.h" // HistManagerBase,
 // edm::ParameterSet, TFileDirectory

#include <boost/range/adaptor/map.hpp> // boost::adaptors::map_keys
#include <boost/range/algorithm/copy.hpp> // boost::copy()

#include <typeinfo> // typeid()
#include <algorithm> // std::set_intersection()

template <typename FloatType_class,
          typename IntType_class>
struct NtupleFillerBDT
  : public HistManagerBase
{
  typedef FloatType_class float_type;
  typedef IntType_class   int_type;

private:
  template <typename NumericType>
  struct BasicFillerWCounter
    : BasicFiller<NumericType>
  {
    BasicFillerWCounter()
      : BasicFiller<NumericType>()
      , isFilled(false)
    {}

    BasicFillerWCounter(const std::string & branchName)
      : BasicFiller<NumericType>(branchName)
      , isFilled(false)
    {}

    template <typename OtherType>
    void
    setValue(OtherType val,
             bool isFilled_value = true)
    {
      BasicFiller<NumericType>::setValue(val);
      isFilled = isFilled_value;
    }

    void
    reset()
    {
      setValue<NumericType>(PLACEHOLDER, false);
    }

    bool isFilled;
  };

public:
  NtupleFillerBDT(edm::ParameterSet const & cfg,
                  bool assert_only_once = true)
    : HistManagerBase(cfg)
    , tree_(nullptr)
    , assert_only_once_(assert_only_once)
    , fill_assert_(false)
  {}

  template <typename NumericType,
            typename = typename std::enable_if<std::is_arithmetic<NumericType>::value>::type>
  void
  register_variable(const std::string & key)
  {
    if(std::is_same<NumericType, FloatType_class>::value)
      float_map_[key] = BasicFillerWCounter<FloatType_class>(key);
    else if(std::is_same<NumericType, IntType_class>::value)
      int_map_[key] = BasicFillerWCounter<IntType_class>(key);
    else
      throw std::invalid_argument(std::string("unsupported type: ") + typeid(NumericType).name());
  }

  template <typename NumericType,
            typename = typename std::enable_if<std::is_arithmetic<NumericType>::value>::type,
            typename... T>
  void
  register_variable(const std::string & key,
                    const T &... rest)
  {
    if(std::is_same<NumericType, FloatType_class>::value)
      float_map_[key] = BasicFillerWCounter<FloatType_class>(key);
    else if(std::is_same<NumericType, IntType_class>::value)
      int_map_[key] = BasicFillerWCounter<IntType_class>(key);
    else
      throw std::invalid_argument(std::string("unsupported type: ") + typeid(NumericType).name());
    register_variable<NumericType>(rest...);
  }

  void
  bookTree(TFileDirectory & dir)
  {
    // assert that none of the maps have common keys
    std::vector<std::string> float_keys, int_keys, common_keys;
    boost::copy(float_map_ | boost::adaptors::map_keys, std::back_inserter(float_keys));
    boost::copy(int_map_   | boost::adaptors::map_keys, std::back_inserter(int_keys  ));
    std::sort(float_keys.begin(), float_keys.end());
    std::sort(int_keys.begin(), int_keys.end());
    std::set_intersection(
      float_keys.begin(), float_keys.end(), int_keys.begin(), int_keys.end(),
      std::back_inserter(common_keys)
    );

    TDirectory * subDir = createHistogramSubdirectory(dir);
    subDir -> cd();
    tree_ = new TTree("evtTree", "evtTree");

    for(auto & kv: float_map_)
      kv.second.initBranch(tree_);
    for(auto & kv: int_map_)
      kv.second.initBranch(tree_);
  }

  NtupleFillerBDT &
  operator()(const std::string & key,
             double value) // the variable will be converted to respective type anyways
  {
    if(float_map_.count(key))
      float_map_[key].setValue(value);
    else if(int_map_.count(key))
      int_map_[key].setValue(value);
    else
      throw std::invalid_argument(std::string("No such key: ") + key);
    return *this;
  }

  void
  fill()
  {
    if(! fill_assert_)
    {
      if(! assert_fill(float_map_) || ! assert_fill(int_map_))
        throw std::invalid_argument("Not all values in the map have been filled!");
      if(assert_only_once_)
        fill_assert_ = true;
    }

    tree_ -> Fill();

    for(auto & kv: float_map_)
      kv.second.reset();
    for(auto & kv: int_map_)
      kv.second.reset();
  }

  void
  fill(const std::map<std::string, FloatType_class> & m_float,
       const std::map<std::string, IntType_class>   & m_int)
  {
    fill(float_map_, m_float);
    fill(int_map_,   m_int);

    fill();
  }

  void
  bookHistograms(__attribute__((unused)) TFileDirectory & dir) override
  {}

private:
  TTree * tree_;

  template <typename NumericType,
            typename = typename std::enable_if<std::is_arithmetic<NumericType>::value>::type>
  static bool
  assert_fill(const std::map<std::string, BasicFillerWCounter<NumericType>> & m)
  {
    return std::all_of(
      std::begin(m), std::end(m), [](const auto & value) -> bool
      {
        return value.second.isFilled == true;
      }
    );
  }

  template <typename NumericType,
            typename = typename std::enable_if<std::is_arithmetic<NumericType>::value>::type>
  void
  fill(std::map<std::string, BasicFillerWCounter<NumericType>> & lhs,
       const std::map<std::string, NumericType> & rhs)
  {
    for(const auto & kv: rhs)
    {
      if(! lhs.count(kv.first))
        throw std::invalid_argument(std::string("No such key: ") + kv.first);
      lhs[kv.first].setValue(kv.second);
    }
  }

  std::map<std::string, BasicFillerWCounter<FloatType_class>> float_map_;
  std::map<std::string, BasicFillerWCounter<IntType_class>>   int_map_;

  bool assert_only_once_;
  bool fill_assert_;
};

#endif // NTUPLEFILLERBDT_H