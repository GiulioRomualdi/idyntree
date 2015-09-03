classdef AngularMotionVector3Semantics < iDynTree.GeomVector3Semantics__AngularMotionVector3Semantics
  methods
    function self = AngularMotionVector3Semantics(varargin)
      self@iDynTree.GeomVector3Semantics__AngularMotionVector3Semantics('_swigCreate');
      if nargin~=1 || ~ischar(varargin{1}) || ~strcmp(varargin{1},'_swigCreate')
        % How to get working on C side? Commented out, replaed by hack below
        %self.swigInd = iDynTreeMATLAB_wrap(301, varargin{:});
        tmp = iDynTreeMATLAB_wrap(301, varargin{:}); % FIXME
        self.swigInd = tmp.swigInd;
        tmp.swigInd = uint64(0);
      end
    end
    function delete(self)
      if self.swigInd
        iDynTreeMATLAB_wrap(302, self);
        self.swigInd=uint64(0);
      end
    end
  end
  methods(Static)
  end
end
