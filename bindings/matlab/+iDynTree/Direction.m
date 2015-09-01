classdef Direction < iDynTree.Vector3
  methods
    function self = Direction(varargin)
      self@iDynTree.Vector3('_swigCreate');
      if nargin~=1 || ~ischar(varargin{1}) || ~strcmp(varargin{1},'_swigCreate')
        % How to get working on C side? Commented out, replaed by hack below
        %self.swigInd = iDynTreeMATLAB_wrap(226, varargin{:});
        tmp = iDynTreeMATLAB_wrap(226, varargin{:}); % FIXME
        self.swigInd = tmp.swigInd;
        tmp.swigInd = uint64(0);
      end
    end
    function delete(self)
      if self.swigInd
        iDynTreeMATLAB_wrap(227, self);
        self.swigInd=uint64(0);
      end
    end
    function varargout = Normalize(self,varargin)
      [varargout{1:nargout}] = iDynTreeMATLAB_wrap(228, self, varargin{:});
    end
    function varargout = toString(self,varargin)
      [varargout{1:max(1,nargout)}] = iDynTreeMATLAB_wrap(229, self, varargin{:});
    end
    function varargout = display(self,varargin)
      [varargout{1:max(1,nargout)}] = iDynTreeMATLAB_wrap(230, self, varargin{:});
    end
  end
  methods(Static)
  end
end