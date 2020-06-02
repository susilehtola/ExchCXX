#pragma once

#include <exchcxx/xc_kernel.hpp>
#include <exchcxx/impl/builtin/fwd.hpp>
#include <type_traits>

namespace ExchCXX {



class BuiltinKernel {

  XCKernel::Spin polar_;

public:

  explicit BuiltinKernel( XCKernel::Spin p ) : polar_(p) { }
  virtual ~BuiltinKernel() = default;

  virtual bool is_lda()       const noexcept = 0;
  virtual bool is_gga()       const noexcept = 0;
  virtual bool is_mgga()      const noexcept = 0;
  virtual bool is_hyb()       const noexcept = 0;
  virtual double hyb_exx()    const noexcept = 0;

  inline bool is_polarized() const noexcept { 
    return polar_ == XCKernel::Spin::Polarized; 
  };

  inline auto polar() const noexcept { return polar_; }

  // LDA interface
  virtual LDA_EXC_GENERATOR( eval_exc )         const;
  virtual LDA_EXC_VXC_GENERATOR( eval_exc_vxc ) const;

  // GGA interface
  virtual GGA_EXC_GENERATOR( eval_exc )         const;
  virtual GGA_EXC_VXC_GENERATOR( eval_exc_vxc ) const;

  // MGGA interface
  virtual MGGA_EXC_GENERATOR( eval_exc )         const;
  virtual MGGA_EXC_VXC_GENERATOR( eval_exc_vxc ) const;

#ifdef EXCHCXX_ENABLE_DEVICE

  // LDA interface
  virtual LDA_EXC_GENERATOR( eval_exc_device )         const;
  virtual LDA_EXC_VXC_GENERATOR( eval_exc_vxc_device ) const;
  virtual LDA_EXC_GENERATOR_DEVICE( eval_exc_device_async )         const;
  virtual LDA_EXC_VXC_GENERATOR_DEVICE( eval_exc_vxc_device_async ) const;

  // GGA interface
  virtual GGA_EXC_GENERATOR( eval_exc_device )         const;
  virtual GGA_EXC_VXC_GENERATOR( eval_exc_vxc_device ) const;
  virtual GGA_EXC_GENERATOR_DEVICE( eval_exc_device_async )         const;
  virtual GGA_EXC_VXC_GENERATOR_DEVICE( eval_exc_vxc_device_async ) const;

  // MGGA interface
  virtual MGGA_EXC_GENERATOR( eval_exc_device )         const;
  virtual MGGA_EXC_VXC_GENERATOR( eval_exc_vxc_device ) const;
  virtual MGGA_EXC_GENERATOR_DEVICE( eval_exc_device_async )         const;
  virtual MGGA_EXC_VXC_GENERATOR_DEVICE( eval_exc_vxc_device_async ) const;

#endif


};



namespace detail {

template <typename KernelType>
LDA_EXC_GENERATOR( host_eval_exc_helper );
template <typename KernelType>
LDA_EXC_VXC_GENERATOR( host_eval_exc_vxc_helper );

template <typename KernelType>
GGA_EXC_GENERATOR( host_eval_exc_helper );
template <typename KernelType>
GGA_EXC_VXC_GENERATOR( host_eval_exc_vxc_helper );





template <typename KernelType>
struct BuiltinKernelImpl<
  KernelType,
  std::enable_if_t<kernel_traits<KernelType>::is_lda>
> : public BuiltinKernel {

  using traits = kernel_traits<KernelType>;

  BuiltinKernelImpl( XCKernel::Spin p ) : BuiltinKernel(p) { }
  virtual ~BuiltinKernelImpl() noexcept = default;

  inline bool is_hyb()  const noexcept override { return traits::is_hyb;  }
  inline bool is_lda()  const noexcept override { return traits::is_lda;  }
  inline bool is_gga()  const noexcept override { return traits::is_gga;  }
  inline bool is_mgga() const noexcept override { return traits::is_mgga; }

  inline double hyb_exx() const noexcept override {
    return traits::is_hyb ? traits::exx_coeff : 0.;
  }

  inline FORWARD_XC_ARGS( LDA, EXC, eval_exc, 
    host_eval_exc_helper<KernelType>, const override );
  inline FORWARD_XC_ARGS( LDA, EXC_VXC, eval_exc_vxc, 
    host_eval_exc_vxc_helper<KernelType>, const override );

};



template <typename KernelType>
struct BuiltinKernelImpl<
  KernelType,
  std::enable_if_t<kernel_traits<KernelType>::is_gga>
> : public BuiltinKernel {

  using traits = kernel_traits<KernelType>;

  BuiltinKernelImpl( XCKernel::Spin p ) : BuiltinKernel(p) { }
  virtual ~BuiltinKernelImpl() noexcept = default;

  inline bool is_hyb()  const noexcept override { return traits::is_hyb;  }
  inline bool is_lda()  const noexcept override { return traits::is_lda;  }
  inline bool is_gga()  const noexcept override { return traits::is_gga;  }
  inline bool is_mgga() const noexcept override { return traits::is_mgga; }

  inline double hyb_exx() const noexcept override {
    return traits::is_hyb ? traits::exx_coeff : 0.;
  }

  inline FORWARD_XC_ARGS( GGA, EXC, eval_exc, 
    host_eval_exc_helper<KernelType>, const override );
  inline FORWARD_XC_ARGS( GGA, EXC_VXC, eval_exc_vxc, 
    host_eval_exc_vxc_helper<KernelType>, const override );

};

}





}