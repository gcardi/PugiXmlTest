//---------------------------------------------------------------------------

#ifndef GridMapperH
#define GridMapperH

#include <vector>
#include <numeric>
#include <algorithm>
#include <memory>
#include <iterator>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

//---------------------------------------------------------------------------

template<typename G, typename T = int, typename R = T>
inline
R ColNum2Idx( G const & Grid, T ACol ) {
    return ACol - const_cast<G&>( Grid ).FixedCols;
}
//---------------------------------------------------------------------------

template<typename G, typename T = int, typename R = T>
inline
R RowNum2Idx( G const & Grid, T ARow ) {
    return ARow - const_cast<G&>( Grid ).FixedRows;
}
//---------------------------------------------------------------------------

template<typename CT = int>
class MoveableColGridMapper
{
public:
    explicit MoveableColGridMapper( size_t ColCount );
    int Num2Idx( int ACol ) const { return DoNum2Idx( ACol ); }
    void ColumnMoved( int FromCol, int ToCol ) { DoColumnMoved( FromCol, ToCol );	}
    int Idx2Num( int Idx ) const { return DoIdx2Num( Idx ); }
    void SetColumn( int ACol, CT const & Column );
    size_t GetColCount() const { return cols_.size(); }
    void Swap( MoveableColGridMapper& Other ) { Other.cols_.swap( cols_ ); }
protected:
    virtual int DoNum2Idx( int ARow ) const;
    virtual void DoColumnMoved( int FromCol, int ToCol );
    virtual int DoIdx2Num( int Idx ) const;
private:
    using ColsCont = std::vector<CT>;

    ColsCont cols_;
};
//---------------------------------------------------------------------------

using ColumnGridMapper = MoveableColGridMapper<>;

//---------------------------------------------------------------------------

template<typename CT>
void MoveableColGridMapper<CT>::SetColumn( int ACol, CT const & Column )
{
    cols_[ACol] = Column;
}
//---------------------------------------------------------------------------

template<typename CT>
MoveableColGridMapper<CT>::MoveableColGridMapper( size_t ColCount )
  : cols_( ColCount )
{
    std::iota( std::begin( cols_ ), std::end( cols_ ), CT() );
}
//---------------------------------------------------------------------------

template<typename CT>
int MoveableColGridMapper<CT>::DoNum2Idx( int ACol ) const
{
    return cols_[ACol];
}
//---------------------------------------------------------------------------

template<typename CT>
void MoveableColGridMapper<CT>::DoColumnMoved( int FromCol, int ToCol )
{
    typename ColsCont::value_type Tmp = cols_[FromCol];
    typename ColsCont::iterator DestBegin = std::begin( cols_ );
    advance( DestBegin, FromCol );
    typename ColsCont::iterator SourceBegin = DestBegin;
    if ( FromCol < ToCol ) {
        typename ColsCont::iterator SourceEnd = ++SourceBegin;
        std::advance( SourceEnd, ToCol - FromCol );
        std::copy( SourceBegin, SourceEnd, DestBegin );
    }
    else  {
        typename ColsCont::iterator SourceEnd = --SourceBegin;
        std::advance( SourceEnd, ToCol - FromCol );
        while ( SourceBegin != SourceEnd ) { *DestBegin-- = *SourceBegin--; }
    }
    cols_[ToCol] = Tmp;
}
//---------------------------------------------------------------------------

template<typename CT>
int MoveableColGridMapper<CT>::DoIdx2Num( int Idx ) const
{
    return std::distance(
        std::begin( cols_ ), std::find( std::begin( cols_ ),
        std::end( cols_ ), Idx )
    );
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class ViewRowGridMapper
{
public:
    using ToModelType = int;

    ViewRowGridMapper() : mapped_( false ) {}
    int Num2Idx( int ARow ) const { return DoNum2Idx( ARow ); }
    int Idx2Num( int Idx ) const { return DoIdx2Num( Idx ); }
    bool IsMapped() const { return mapped_; }
    void ClearMap();

    int Map( int RowCount );

    template<typename RandomAccessIterator, typename F>
    int Map( RandomAccessIterator Begin, RandomAccessIterator End, F Pred );

    template<typename C>
    void Sort( C CmpPred );

    template<typename T, typename C>
    void Insert( T const & Item, int ModelPos, C CmpPred );

    int GetCount() const { return static_cast<int>( view2Model_.size() ); }
protected:
    virtual int DoNum2Idx( int RelativeRow ) const;
    virtual int DoIdx2Num( int Idx ) const;
private:
    using ViewToModelCont = std::vector<ToModelType>;

    using ModelToViewType = boost::tuple<int,int>;
    enum class ModelToRowField : int { ModelIdx, ViewIdx };
    using ModelToViewCont = std::vector<ModelToViewType>;

    bool mapped_;
    ViewToModelCont view2Model_;
    ModelToViewCont model2view_;

    void BuildModelToView( ViewToModelCont const & View2Model );
};
//---------------------------------------------------------------------------

template<typename RandomAccessIterator, typename F>
int ViewRowGridMapper::Map( RandomAccessIterator Begin, RandomAccessIterator End,
                            F Pred )
{
    ViewToModelCont View2Model;
    View2Model.reserve( std::distance( Begin, End ) );

    for ( int Idx = 0 ; Begin != End ; ++Begin ) {
        if ( Pred( *Begin ) ) {
            View2Model.push_back( Idx );
        }
        ++Idx;
    }

    BuildModelToView( View2Model );

    view2Model_.swap( View2Model );
    mapped_ = true;

    return GetCount();
}
//---------------------------------------------------------------------------

template<typename C>
void ViewRowGridMapper::Sort( C CmpPred )
{
    ViewToModelCont View2Model( view2Model_ );
    std::stable_sort( std::begin( View2Model ), std::end( View2Model ), CmpPred );
    BuildModelToView( View2Model );
    View2Model.swap( view2Model_ );
}
//---------------------------------------------------------------------------

template<typename T, typename C>
void ViewRowGridMapper::Insert( T const & Item, int ModelPos, C CmpPred )
{
//    std::lower_bound(
}

//---------------------------------------------------------------------------
#endif
