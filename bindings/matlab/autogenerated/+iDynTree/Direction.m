classdef Direction < iDynTree.Vector3
  methods
    function self = Direction(varargin)
      self@iDynTree.Vector3(SwigRef.Null);
      if nargin==1 && strcmp(class(varargin{1}),'SwigRef')
        if ~isnull(varargin{1})
          self.swigPtr = varargin{1}.swigPtr;
        end
      else
        tmp = iDynTreeMEX(651, varargin{:});
        self.swigPtr = tmp.swigPtr;
        tmp.SwigClear();
      end
    end
    function varargout = Normalize(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(652, self, varargin{:});
    end
    function varargout = isParallel(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(653, self, varargin{:});
    end
    function varargout = isPerpendicular(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(654, self, varargin{:});
    end
    function varargout = reverse(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(655, self, varargin{:});
    end
    function varargout = toString(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(656, self, varargin{:});
    end
    function varargout = display(self,varargin)
      [varargout{1:nargout}] = iDynTreeMEX(657, self, varargin{:});
    end
    function delete(self)
      if self.swigPtr
        iDynTreeMEX(659, self);
        self.SwigClear();
      end
    end
  end
  methods(Static)
    function varargout = Default(varargin)
     [varargout{1:nargout}] = iDynTreeMEX(658, varargin{:});
    end
  end
end
