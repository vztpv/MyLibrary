#ifndef NEURALNETWORK_H_INCLUDED
#define NEURALNETWORK_H_INCLUDED


#include <wiz/global.h> /// for toString
#include <wiz/newArrays.h>
#include <random>
#include <ctime>

namespace wiz
{
    // namespace NN
     // 0 or 1
    template <typename INTEGER, typename NUMBER>
    INTEGER up(const NUMBER nx) /// move to main.cpp?
    {
        int ix = nx + 0.5;/// + 0.5;

        //if( nx > 0.1 ) { ix = 1; }
        //else { ix = 0; }
        return ix;
    }
     /// chk...
    /*template <typename INTEGER>
    Array<INTEGER> Convert( const INTEGER ival ) /// assume ival >= 0
    {
        Array<INTEGER> iarr;
        string str = wiz::toStr(ival, 2);
        iarr = Array<INTEGER>(str.size(), 0);
        for (int i = 0; i < str.size(); i++)
        {
            iarr[i] = str[i] - '0';
        }
        return iarr;
    }*/
    /// namespace NeuralNetwork?
    /// Neural Net
    // IH  : input_layer->hidden_layer
    // HO : hidden_layer -> output_layer
    // output is list of ( 0 or 1 ).
    template < typename INTEGER, typename NUMBER, class RAND, class RAND2,
            class G, class DG, class G2=G, class DG2=DG >
                    /// DG - need rename!!??
    class NeuralNet
    {
    private:
        SecondArray<NUMBER> weightIH; /// input_layer -> hidden_layer
        SecondArray<NUMBER> weightHO; /// hidden_layer -> output_layer

        SecondArray<NUMBER> prevDeltaIH;
        SecondArray<NUMBER> prevDeltaHO;

        default_random_engine random_engine;
    public:
        NUMBER eta; /// ToDo make - set, get
        NUMBER eta2; // learning rate.
        NUMBER MOMENTUM; /// added... why use??
        NUMBER H; /// remove??
        INTEGER INPUT_NODE_NUM;
        INTEGER HIDDEN_NODE_NUM;
        INTEGER OUTPUT_NODE_NUM;

        NUMBER GetEta()const { return eta; }
        NUMBER GetEta2()const { return eta2; }
        NUMBER GetH()const { return H; }
        SecondArray<NUMBER> GetWeightIH()const { return weightIH; }  /// NUMBER get( i, j )???
        SecondArray<NUMBER> GetWeightHO()const { return weightHO; }
        void SetWeightIH( const INTEGER row, const INTEGER col, const NUMBER weight )
        {
            weightIH[row][col] = weight;
        }
        void SetWeightHO( const INTEGER row, const INTEGER col, const NUMBER weight )
        {
            weightHO[row][col] = weight;
        }
     private:
        template <class INITWEIGHT>
        void InitWeight(SecondArray<NUMBER>& arr )
        {
            const int rowSize = arr.getRowN();
            const int colSize = arr.getColumnN();

            for( int i = 0; i < rowSize; ++i ) {
                for( int j = 0; j < colSize; ++j ) {
                    arr[i][j] = INITWEIGHT() ( rowSize, random_engine );
                }
            }
        }



        NUMBER a(const Array<NUMBER>& weight, const Array<NUMBER>& input,
                    const INTEGER start, const INTEGER last)const /// int 대신 INTEGER??
        {
            NUMBER sum = 0;

            for ( INTEGER i = start; i <= last; i++) {
                sum = sum + weight[i] * input[i];
            }

            return sum;
        }
        /// weight -> 각 input마다의 weight!
        NUMBER o(const Array<NUMBER>& weight,
                        const Array<NUMBER>& input, const INTEGER start, const INTEGER last ) const
                        // output
        {
            return G()(a(weight, input, start, last));
        }
        NUMBER o2(const Array<NUMBER>& weight,
                        const Array<NUMBER>& input, const INTEGER start, const INTEGER last ) const
                        // output
        {
            return G2()(a(weight, input, start, last));
        }

        NUMBER delta_wij( const NUMBER delta_j, const NUMBER x_ij) const {
            return delta_j * x_ij;
        }


    public:
        explicit NeuralNet( const INTEGER input_node_num, const INTEGER hidden_node_num, const INTEGER output_node_num,
                            const NUMBER eta = 0.05, const NUMBER eta2 = 0.01, const NUMBER h = 0.01,
                            const NUMBER momentum = 0.05 ) /// ??
        : INPUT_NODE_NUM( input_node_num ), HIDDEN_NODE_NUM( hidden_node_num ), OUTPUT_NODE_NUM( output_node_num ),
                                                                    eta( eta ), eta2( eta2 ), H( h ), MOMENTUM( momentum )
        {
            random_engine.seed( time(NULL) );

			weightIH = SecondArray<NUMBER>(HIDDEN_NODE_NUM, INPUT_NODE_NUM+1, 0);
			weightHO = SecondArray<NUMBER>(OUTPUT_NODE_NUM, HIDDEN_NODE_NUM+1, 0);

			prevDeltaIH = SecondArray<NUMBER>(HIDDEN_NODE_NUM, INPUT_NODE_NUM+1, 0);
			prevDeltaHO = SecondArray<NUMBER>(OUTPUT_NODE_NUM, HIDDEN_NODE_NUM+1, 0);

            InitWeight<RAND>( weightIH );
            InitWeight<RAND2>( weightHO );
        }
        /// void reset()?

        void GetOutput( const Array<NUMBER>& _input, Array<NUMBER>& output) const
        {
            Array<NUMBER> _hidden(HIDDEN_NODE_NUM, 0);
            Array<NUMBER> input(INPUT_NODE_NUM+1, 1);
            Array<NUMBER> hidden(HIDDEN_NODE_NUM+1, 1);
            // void GetOutput( weightIH, weightHo, input, out output )
            // 1. input node -> hidden node
            for( int i=0; i < INPUT_NODE_NUM; i++ )
            {
                input[i+1] = _input[i];
            }
            for (int i = 0; i < HIDDEN_NODE_NUM; i++) {
                _hidden[i] = o(weightIH[i], input, 0, INPUT_NODE_NUM);
            }
            // 2. hidden node -> output node
            for( int i=0; i < HIDDEN_NODE_NUM; i++ ) {
                hidden[i+1] = _hidden[i];
            }
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) {
                output[i] = o2(weightHO[i], hidden, 0, HIDDEN_NODE_NUM);
            }
        }
        void Train(
                   const Array<NUMBER>& _input, /// const Array<NUMBER>& output,
                   const Array<NUMBER>& truthValue // INTEGER?
                   )
        {
            Array<NUMBER> input(INPUT_NODE_NUM+1, 1);

            for( int i=1; i<= INPUT_NODE_NUM; ++i ) /// shift, mem_move??
            {
                input[i] = _input[i-1];
            }
            Array<NUMBER> _hidden(HIDDEN_NODE_NUM, 0);
            Array<NUMBER> hidden(HIDDEN_NODE_NUM+1, 1);
            Array<NUMBER> output(OUTPUT_NODE_NUM, 0);
            Array<NUMBER> deltaH(HIDDEN_NODE_NUM, 0);
            Array<NUMBER> deltaK(OUTPUT_NODE_NUM, 0);

            // Compute output
            // void GetOutput( weightIH, weightHo, input, out output )
            // 1. input node -> hidden node
            for (int i = 0; i < HIDDEN_NODE_NUM; i++) {
                _hidden[i] = o( weightIH[i], input, 0, INPUT_NODE_NUM );
            }
            for( int i=1; i <= HIDDEN_NODE_NUM; ++i ) {
                hidden[i] = _hidden[i-1];
            }
            // 2. hidden node -> output node
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) {
				output[i] = o2(weightHO[i], hidden, 0, HIDDEN_NODE_NUM );
            }

            // Calcul delta value
            // 1. delta k
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) {
				deltaK[i] = DG2()( output[i] ) * (truthValue[i] - output[i]);
                        /// -DG2, weight~ -= ~, weight~ -= ~.
            }
            // 2. delta h // delta j
            {
                Array<NUMBER> sum(HIDDEN_NODE_NUM, 0); /// chk!!
                //sum.reset( 0 ); /// cf) memory reset?

                for (int k = 0; k < OUTPUT_NODE_NUM; k++) {
                    for (int h = 0; h < HIDDEN_NODE_NUM; h++) { //
                        sum[h] = sum[h] + weightHO[k][h+1] * deltaK[k];
                    }
                }
                for (int h = 0; h < HIDDEN_NODE_NUM; h++) {
                    deltaH[h] = DG()( hidden[h+1] ) * sum[h];
                }
/*
                 for( int h = 0; h < HIDDEN_NODE_NUM; h++ ) {
                      NUMBER sum = 0;
                    for( int k = 0; k < OUTPUT_NODE_NUM; k++ ) {
                          sum = sum + weightHO[k][h+1] * deltaK[k];
                      }
                      deltaH[h]  = DG()( hidden[h+1] ) * sum;
                 }
*/
            }

            // Update each network weight w(i,j)
            // eta? = 0.005, 0.05?
            // 1. update input node -> hidden node
            for (int i = 0; i < HIDDEN_NODE_NUM; i++) {
                for (int j = 0; j <= INPUT_NODE_NUM; j++) {
                    NUMBER delta = eta * delta_wij(deltaH[i], input[j]); /// ToDo...
                    weightIH[i][j] = weightIH[i][j] + delta;
                    weightIH[i][j] = weightIH[i][j] + MOMENTUM * prevDeltaIH[i][j];
                    prevDeltaIH[i][j] = delta;
                }
            }
            // 2. hidden node -> output node
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) { //
                for (int j = 0; j <= HIDDEN_NODE_NUM; j++) {
                    NUMBER delta = eta2 * delta_wij(deltaK[i], hidden[j]);
                    weightHO[i][j] = weightHO[i][j] + delta;
                    weightHO[i][j] = weightHO[i][j] + MOMENTUM * prevDeltaHO[i][j];
                    prevDeltaHO[i][j] = delta;
                }
            }
        }

   };

}



#endif // NEURALNETWORK_H_INCLUDED
