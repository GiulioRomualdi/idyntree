function varargout = LINK_INVALID_INDEX(varargin)
  narginchk(0,1)
  if nargin==0
    nargoutchk(0,1)
    varargout{1} = iDynTreeMATLAB_wrap(332);
  else
    nargoutchk(0,0)
    iDynTreeMATLAB_wrap(333,varargin{1});
  end
end