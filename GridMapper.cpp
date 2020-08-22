//---------------------------------------------------------------------------

#pragma hdrstop

#include <algorithm>
#include <numeric>
#include <iterator>

#include <boost/tuple/tuple.hpp>

#include "GridMapper.h"

using std::lower_bound;
using std::iota;
using std::for_each;
using std::stable_sort;
using std::begin;
using std::end;

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

void ViewRowGridMapper::ClearMap()
{
    mapped_ = false;
    view2Model_.clear();
    model2view_.clear();
}
//---------------------------------------------------------------------------

int ViewRowGridMapper::Map( int RowCount )
{
    ViewToModelCont View2Model( RowCount );
    iota( begin( View2Model ), end( View2Model ), 0 );

    BuildModelToView( View2Model );

    view2Model_.swap( View2Model );
    mapped_ = true;
    return GetCount();
}
//---------------------------------------------------------------------------

template<size_t TupleFieldIdx>
struct ModelIndexCompare {
    template<typename T, typename U>
    bool operator()( T const & Tuple, U Value ) const {
        return Tuple.template get<TupleFieldIdx>() < Value;
    }
};

int ViewRowGridMapper::DoIdx2Num( int Idx ) const
{
    if ( mapped_ ) {
        ModelToViewCont::const_iterator It =
            lower_bound(
                begin( model2view_ ), end( model2view_ ), Idx,
                ModelIndexCompare<(size_t)ModelToRowField::ModelIdx>()
            );
        if ( It != end( model2view_ ) &&
             !( Idx < It->get<(int)ModelToRowField::ModelIdx>() ) )
        {
            return It->get<(size_t)ModelToRowField::ViewIdx>();
        }
        return -1;
    }
    else {
        return Idx;
    }
}
//---------------------------------------------------------------------------

int ViewRowGridMapper::DoNum2Idx( int ARow ) const
{
    return mapped_ && ARow < static_cast<int>( view2Model_.size() ) ?
             view2Model_[ARow]
           :
             ARow;
}
//---------------------------------------------------------------------------

template<typename C>
class CopyIotaType {
public:
    explicit CopyIotaType( C& Cont ) : n_( 0 ), cont_( Cont ) {}

    template<typename T>
    void operator()( T const & Val ) {
        cont_.push_back( boost::make_tuple( Val, n_++ ) );
    }
private:
    int n_;
    C& cont_;
};

template<typename C>
inline CopyIotaType<C> CopyIota( C& Cont ) { return CopyIotaType<C>( Cont ); }

void ViewRowGridMapper::BuildModelToView( ViewToModelCont const & View2Model )
{
    ModelToViewCont Model2View;
    Model2View.reserve( View2Model.size() );
    for_each( begin( View2Model ), end( View2Model ), CopyIota( Model2View ) );
    stable_sort( begin( Model2View ), end( Model2View ) );
    model2view_.swap( Model2View );
}
//---------------------------------------------------------------------------

