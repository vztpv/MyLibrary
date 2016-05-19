#ifndef NEURALNETWORK_H_INCLUDED
#define NEURALNETWORK_H_INCLUDED


#include <wiz/global.h> /// for toString
#include <wiz/newArrays.h>

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
    /// namespace NeuralNetwork?
    /// Neural Net
    // IH  : input_layer->hidden_layer
    // HO : hidden_layer -> output_layer
    // output is list of ( 0 or 1 ).
    /// first goal is make one class
    template < typename INTEGER, typename NUMBER, class RAND, class G >
    class NeuralNet
    {
    private:
        SecondArray<NUMBER> weightIH; /// input_layer -> hidden_layer
        SecondArray<NUMBER> weightHO; /// hidden_layer -> output_layer
    public:
        NUMBER GAMMA; /// ToDo make - set, get
        NUMBER H; /// remove??
        INTEGER INPUT_NODE_NUM;
        INTEGER HIDDEN_NODE_NUM;
        INTEGER OUTPUT_NODE_NUM;
        NUMBER GetGAMMA()const { return GAMMA; }
        NUMBER GetH()const { return H; }
        SecondArray<NUMBER> GetWeightIH()const { return weightIH; }
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
        void InitWeight(Array<NUMBER>& arr)
        {
            const int size = arr.size();

            for (int i = 0; i < size; i++) {
                arr[i] = RAND()();
            }
        }
        void InitWeight(SecondArray<NUMBER>& arr)
        {
            const int rowSize = arr.getRowN();

            for (int i = 0; i < rowSize; i++) {
                InitWeight(arr[i]);
            }
        }

        NUMBER Sum(const Array<NUMBER>& arr) const
        {
            NUMBER sum = 0;
            const int size = arr.size();

            for (int i = 0; i < size; i++)
            {
                sum = sum + arr[i];
            }

            return sum;
        }
        NUMBER Sum(const SecondArray<NUMBER>& arr) const
        {
            NUMBER sum = 0;
            const int rowSize = arr.getRowN();

            for (int i = 0; i < rowSize; i++)
            {
                sum = sum + Sum(arr[i]);
            }

            return sum;
        }

        /// chk...
        Array<INTEGER> Convert( const INTEGER ival )const /// assume ival >= 0
        {
            Array<INTEGER> iarr;
            string str = wiz::toStr(ival, 2);
            iarr = Array<INTEGER>(str.size(), 0);
            for (int i = 0; i < str.size(); i++)
            {
                iarr[i] = str[i] - '0';
            }
            return iarr;
        }
        NUMBER a(const Array<NUMBER>& weight, const Array<NUMBER>& input,
                    const int num)const /// int 대신 INTEGER??
        {
             NUMBER sum = 0;

            for ( int i = 0; i < num; i++) {
                sum = sum + weight[i] * input[i];
            }

            return sum;
        }
        /// weight -> 각 input마다의 weight!
        NUMBER o(const Array<NUMBER>& weight,
                        const Array<NUMBER>& input, const int num) const
                        // output
        {
            return G()(a(weight, input, num));
        }

        NUMBER delta_wij(const NUMBER delta_j, const NUMBER x_ij) const {
            return GAMMA * delta_j * x_ij;
        }


    public:
        explicit NeuralNet( const INTEGER input_node_num, const INTEGER hidden_node_num, const INTEGER output_node_num,
                           const NUMBER gamma = 0.005, const NUMBER h = 0.01 )
        : INPUT_NODE_NUM( input_node_num ), HIDDEN_NODE_NUM( hidden_node_num ), OUTPUT_NODE_NUM( output_node_num ),
                                                                    GAMMA( gamma ), H( h )
        {
            /// init()?
			weightIH = SecondArray<NUMBER>(HIDDEN_NODE_NUM, INPUT_NODE_NUM, 0);
			weightHO = SecondArray<NUMBER>(OUTPUT_NODE_NUM, HIDDEN_NODE_NUM, 0);
            InitWeight( weightIH );
            InitWeight( weightHO );
        }
        /// void reset()?

        void GetOutput( const Array<NUMBER>& input, Array<NUMBER>& output) const
        {
            //const int HIDDEN_NODE_NUM = weightIH.getRowN();
            //const int INPUT_NODE_NUM = input.size();
            //const int OUTPUT_NODE_NUM = output.size();
            Array<NUMBER> hidden(HIDDEN_NODE_NUM, 0);

            // void GetOutput( weightIH, weightHo, input, out output )
            // 1. input node -> hidden node
            for (int i = 0; i < HIDDEN_NODE_NUM; i++) {
                hidden[i] = o(weightIH[i], input, INPUT_NODE_NUM);
            }
            // 2. hidden node -> output node
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) {
                output[i] = o(weightHO[i], hidden, HIDDEN_NODE_NUM);
            }

        }
        void Train(
                   const Array<NUMBER>& input, /// const Array<NUMBER>& output,
                   const Array<NUMBER>& truthValue // INTEGER?
                   )
        {
            /// chk, 크기 비교...
            //const int HIDDEN_NODE_NUM = weightIH.getRowN();
            //const int OUTPUT_NODE_NUM = output.size();
            //const int INPUT_NODE_NUM = input.size();

            Array<NUMBER> hidden(HIDDEN_NODE_NUM, 0);
            Array<NUMBER> output(OUTPUT_NODE_NUM, 0 );
            Array<NUMBER> deltaH(HIDDEN_NODE_NUM, 0);
            Array<NUMBER> deltaK(weightHO.getRowN(), 0); // OUTPUT_NODE_NUM

            // Compute output
            // void GetOutput( weightIH, weightHo, input, out output )
            // 1. input node -> hidden node
            for (int i = 0; i < HIDDEN_NODE_NUM; i++) {
                hidden[i] = o(weightIH[i], input, INPUT_NODE_NUM);
            }
            // 2. hidden node -> output node
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) {
				output[i] = o(weightHO[i], hidden, HIDDEN_NODE_NUM);
            }
            // Calcul delta value
            // 1. delta k
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) {
				deltaK[i] = output[i] * (1 - output[i]) * (truthValue[i] - output[i]);
            }
            // 2. delta h
                {	// 좀더 캐쉬미스가 적도록 최적화?
                    Array<NUMBER> sum(HIDDEN_NODE_NUM, 0);

                    for (int k = 0; k < OUTPUT_NODE_NUM; k++) {
                        for (int h = 0; h < HIDDEN_NODE_NUM; h++) { //
                            sum[h] = sum[h] + weightHO[k][h] * deltaK[k];
                        }
                    }
                    for (int h = 0; h < HIDDEN_NODE_NUM; h++) {
                        deltaH[h] = hidden[h] * (1 - hidden[h]) * sum[h];
                    }
                    /*
                     // 백업
                     for( int h=0; h < HIDDEN_NODE_NUM; h++ ) {
                         NUMBER sum = 0;
                         for( int k=0; k < OUTPUT_NODE_NUM; k++ ) {
                            sum = sum + weightHO[k][h] * deltaK[k];
                         }
                         deltaH[h] = hidden[h] * ( 1 - hidden[h] ) * sum;
                     } */

                }

            // Update each network weight w(i,j)
            // gammar? = 0.005, 0.05
            // 1. update input node -> hidden node
            for (int i = 0; i < HIDDEN_NODE_NUM; i++) {
                for (int j = 0; j < INPUT_NODE_NUM; j++) {
                    weightIH[i][j] = weightIH[i][j] + delta_wij(deltaH[i], input[j]);
                }
            }
            // 2. hidden node -> output node
            for (int i = 0; i < OUTPUT_NODE_NUM; i++) { // 이름과 순서가 반대임에 주의!
                for (int j = 0; j < HIDDEN_NODE_NUM; j++) {
                    weightHO[i][j] = weightHO[i][j] + delta_wij(deltaK[i], hidden[j]);
                }
            }
        }
        /// To Do - sumOfWeight = Sum( weightIH ) + Sum( weightHO );
        double SumOfWeight()const
        {
            return Sum( weightIH ) + Sum( weightHO );
        }
   };

}



#endif // NEURALNETWORK_H_INCLUDED
