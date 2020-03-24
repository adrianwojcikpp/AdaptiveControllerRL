classdef neuralnetwork
    %NEURALNETWORK Simple object-oriented adaptive neural network 
    %   TODO: Detailed explanation goes here
    
    properties
        L  % number of layers (hidden+output);
        I  % input size
        O  % output size
        NS % network sizes vector (input and layers)
        w  % weigths matrices cell array
        b  % biases vectos cell array
        f  % activation function cell array
        df % activation function derivative cell array
        mxOut % symetric output saturation
    end
    
    methods
        function obj = neuralnetwork(L, NS, AF, MXOUT, SEED)
            %NEURALNETWORK Construct an instance of this class
            %   TODO: Detailed explanation goes here
            
            obj.L = L;           % number of layers
            
            obj.w  = cell(1,L);  % weigths matrices cell array
            obj.b  = cell(1,L);  % biases vectos cell array
            obj.f  = cell(1,L);  % activation function cell array
            obj.df = cell(1,L);  % activation function derivative cell array
            
            obj.NS = NS;         % network sizes vector (input and layers)
            
            obj.I = NS(1);       % input size
            obj.O = NS(end);     % output size
            
            if nargin > 2
                % max output
                obj.mxOut = MXOUT;

                % set random number seed
                randn('seed', SEED)

                for l = 1 : obj.L
                    % activation function & derivative
                    [obj.f{l}, obj.df{l}] = obj.af_selector(AF(l));

                    % PS LEGACY
                    [w,b] = PS_WEIGTHS_GENERATION(NS(l+1), NS(l)+1);

                    % weigths matrix & bias vector
                    obj.w{l} = w;
                    obj.b{l} = b;
                end
            end
        end
        
        function [f,df] = af_selector(~, i)
            %AF_SELECTOR Colection of activation functions 
            %   TODO: Detailed explanation goes here
            switch i
                case 1 % 1 = TANSIG 
                    f  = @tansig;
                    df = @(x)(1 - tansig(x).^2); 
                case 2 % 2 = LOGSIG
                    f  = @logsig;
                    df = @(x)(logsig(x).*(1-logsig(x)));
                case 3 % 3 = LINEAR
                    f  = @purelin;
                    df = @(x)(1);
                otherwise
                    f = [];
                    df = [];
            end
        end
        
        function i = af_index(~, f)
            %AF_INDEX Return index of activation function
            %   TODO: Detailed explanation goes here
            info = functions(f);
            switch info.function
                case 'tansig'  % 1 = TANSIG
                    i = 1; 
                case 'logsig'  % 2 = LOGSIG
                    i = 2;
                case 'purelin' % 3 = LINEAR
                    i = 3;
                otherwise
                    i = nan;
            end
        end
        
        function y = compute(obj, x)
            %COMPUTE Computing neural network output
            %   TODO: Detailed explanation goes here
            output = x;
            for l = 1 : obj.L
                output = obj.f{l}(obj.w{l}*output+obj.b{l});
            end
            y = min(max(output, -obj.mxOut), obj.mxOut);
        end
        
        function obj = backprop_step(obj, Cf, x, eta)
            %BACKPROP_STEP Computing signle step of backpropagation
            %   TODO: Detailed explanation goes here
            
            % variable initialization
            z = cell(obj.L,1);      % sum outputs (z{l} = w*a{l} + b)
            a = cell(obj.L,1);      % activation function outputs (a{l} = f(z{l}), a{0} = x)
            d = cell(obj.L,1);      % layer outputs errors 

            % Forward propagation
            z{1} = obj.w{1} * x + obj.b{1};
            a{1} = obj.f{1}(z{1});
            for l = 2 : obj.L
                z{l} = obj.w{l} * a{l-1} + obj.b{l};
                a{l} = obj.f{l}(z{l});  
            end
            
            % Error backward propagation
            %disp('ERROR')
            d{obj.L} = Cf .* obj.df{obj.L}(z{obj.L});
            %disp( d{obj.L} );
            for l = (obj.L-1) : -1 : 1
                d{l} = obj.w{l+1}' * d{l+1} .* obj.df{l}(z{l});
                %disp( d{l} );
            end
            
            % Weights update
            %disp('WEIGHTS')
            dw = d{1} * x';
            %disp(eta*dw);
            obj.w{1} = obj.w{1} + eta * dw;
            %disp(obj.w{1});
            for l = 2 : obj.L
                dw = d{l} * a{l-1}';
                %disp(eta*dw);
                obj.w{l} = obj.w{l} + eta * dw; 
                %disp(obj.w{l});
            end
            
            % Bias update 
            %disp('BIAS')
            for l = 1 : obj.L
                db = d{l};
                %disp(eta*db);
                obj.b{l} = obj.b{l} + eta * db; 
                %disp(obj.b{l});
            end
        end
        
    end
end

