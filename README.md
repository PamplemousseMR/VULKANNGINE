# VULKANNGINE

@startuml
struct Window
struct Application
struct Instance
struct Surface
struct PhysicalDevice
struct Device
struct Queue
struct Buffer
struct DeviceMemory
struct CommandPool
struct CommandBuffer
struct SwapChain
struct Image
struct ImageView
struct Sampler
struct ShaderModule
struct RenderPass
struct AttachementDescription
struct AttachementReference
struct SubpassDescription
struct SubpassDependency
struct GraphicsPipeline
struct PipelineDepthStencilState
struct PipelineColorBlendState
struct PipelineColorBlendAttachmentState
struct PipelineMultisampleState
struct PipelineRasterizationState
struct PipelineViewportState
struct Viewport
struct Rect2D
struct PipelineInputAssemblyState
struct PipelineVertexInputState
struct VertexInputAttributeDescription
struct VertexInputBindingDescription
struct PipelineLayout
struct DescriptorSetLayout
struct DescriptorSetLayoutBinding
struct PipelineShaderStage 
struct DescriptorPoolSize
struct DescriptorPool
struct DescriptorSet
struct WriteDescriptorSet
struct DescriptorImageInfo
struct DescriptorBufferInfo
struct Framebuffer

Instance x--> "1" Application
Surface x--> "1" Window
Surface x--> "1" Instance
Instance o-- "*" PhysicalDevice
PhysicalDevice o-- "*" Device
Device o-- "*" Queue
Device o-- "*" Buffer
Device o-- "*" DeviceMemory
DeviceMemory -- "*" Buffer
CommandPool x--> "1" Queue
CommandPool o-- "*" CommandBuffer
SwapChain x--> "1" Surface
Device o-- "*" Image
DeviceMemory -- "*" Image
SwapChain o-- "*" Image
ImageView x--> "1" Image
RenderPass o-- "*" AttachementDescription
RenderPass o-- "*" SubpassDescription
RenderPass o-- "*" SubpassDependency
SubpassDescription o-- "*" AttachementReference
SubpassDescription x--> "1" GraphicsPipeline
GraphicsPipeline x--> "1" PipelineDepthStencilState
GraphicsPipeline x--> "1" PipelineColorBlendState
PipelineColorBlendState o-- "*" PipelineColorBlendAttachmentState
GraphicsPipeline x--> "1" PipelineMultisampleState
GraphicsPipeline x--> "1" PipelineRasterizationState
GraphicsPipeline x--> "1" PipelineViewportState
PipelineViewportState o-- "*" Viewport
PipelineViewportState o-- "*" Rect2D
GraphicsPipeline x--> "1" PipelineInputAssemblyState
GraphicsPipeline x--> "1" PipelineVertexInputState
PipelineVertexInputState o-- "*" VertexInputAttributeDescription
PipelineVertexInputState o-- "*" VertexInputBindingDescription
GraphicsPipeline x--> "1" PipelineLayout
PipelineLayout o--> "x" DescriptorSetLayout
DescriptorSetLayout o--> "x" DescriptorSetLayoutBinding
GraphicsPipeline o--> "x" PipelineShaderStage
PipelineShaderStage x--> "1" ShaderModule
DescriptorPool o-- "*" DescriptorPoolSize
DescriptorSet x--> "1" DescriptorPool 
DescriptorSet x--> "1" DescriptorSetLayout
DescriptorSet o-- "*" WriteDescriptorSet
WriteDescriptorSet o-- "*" DescriptorBufferInfo
WriteDescriptorSet o-- "*" DescriptorImageInfo
DescriptorBufferInfo x--> "1" Buffer
DescriptorImageInfo x--> "1" ImageView
DescriptorImageInfo x--> "1" Sampler
Framebuffer x--> "1" RenderPass
Framebuffer o--> "x" ImageView

Device o-- "*" Queue
Device o-- "*" Buffer
Device o-- "*" DeviceMemory
Device o-- "*" Image
Device o-- "*" CommandPool
Device o-- "*" CommandBuffer
Device o-- "*" ImageView
Device o-- "*" Sampler
Device o-- "*" ShaderModule
Device o-- "*" RenderPass
Device o-- "*" GraphicsPipeline
Device o-- "*" DescriptorPool 
Device o-- "*" DescriptorSet
Device o-- "*" DescriptorSetLayout
Device o-- "*" PipelineLayout

@enduml