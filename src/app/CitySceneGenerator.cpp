/**
 * @file CitySceneGenerator.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "CitySceneGenerator.h"

#include "Scene.h"
#include "Material.h"
#include "Common.h"
#include "Helpers.h"
#include "Mesh.h"
#include "Node.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct BuildingVertex
{
	float pos[3];
	float normal[3];
};


static const BuildingVertex buildingVertices[] = {
	{{1.0f, 1.0f, 0.0f}, {0.5773502f, 0.5773502f, 0.5773502f}},
	{{-1.0f, 1.0f, 0.0f}, {-0.5773502f, 0.5773502f, 0.5773502f}},
	{{-1.0f, 1.0f, 2.0f}, {-0.5773502f, 0.5773502f, 0.5773502f}},
	{{1.0f, 1.0f, 2.0f}, {0.5773502f, 0.5773502f, 0.5773502f}},
	{{1.0f, -1.0f, 0.0f}, {0.5773502f,- 0.5773502f, 0.5773502f}},
	{{-1.0f, -1.0f, 0.0f}, {-0.5773502f, -0.5773502f, 0.5773502f}},
	{{-1.0f, -1.0f, 2.0f}, {-0.5773502f, -0.5773502f, 0.5773502f}},
	{{1.0f, -1.0f, 2.0f}, {0.5773502f, -0.5773502f, 0.5773502f}}
};

static const uint32_t buildingIndices[] = {
	0, 1, 2,
	0, 2, 3,
	1, 5, 6,
	1, 6, 2,
	2, 6, 7,
	2, 7, 3,
	3, 7, 4,
	3, 4, 0,
	4, 7, 6,
	4, 6, 5
};

CitySceneGenerator::CitySceneGenerator() {
	std::random_device rd;
	m_rng.seed(rd());
}

void CitySceneGenerator::generate(Scene* scene) {
	auto renderer = scene->renderer();
	auto material = std::make_shared<PhongMaterial>(renderer);
	material->setShader(renderer->shaderManager()->getGlslProgram("phong"));

	PhongMaterialData materialData = { glm::vec4(0.0f, 0.1f, 0.0f, 1.0f), 
		glm::vec4(0.8f, 0.3f, 0.1f, 1.0f), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 5.0f };

	material->properties()->setData(materialData);
	material->properties()->flushData();

	auto mesh = std::make_shared<Mesh>();
	mesh->setPrimitiveType(PrimitiveType::TriangleList);

	size_t buildingVerticesCount = sizeof(buildingVertices) / sizeof(*buildingVertices);
	std::vector<char> vertices(reinterpret_cast<const char*>(buildingVertices), 
		reinterpret_cast<const char*>(buildingVertices) + sizeof(buildingVertices));
	std::vector<VertexElement> layout = create_vector<VertexElement>
		(VertexElement(3, VertexElementType::Float))(VertexElement(3, VertexElementType::Float));
	mesh->loadVertices(vertices, buildingVerticesCount, layout);

	size_t buildingIndicesCount = sizeof(buildingIndices) / sizeof(*buildingIndices);
	std::vector<uint32_t> indices(reinterpret_cast<const unsigned*>(buildingIndices), 
		reinterpret_cast<const unsigned*>(buildingIndices) + buildingIndicesCount);
	mesh->loadIndices(indices);

	size_t numBuildings = 20000;
	float citySize = 1000.0f;
	float minBuildingSize = 10.0f;
	float maxBuildingSize = 60.0f;
	float minHeightToWidthRatio = 8.0f;
	float maxHeightToWidthRatio = 16.0f;

	std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
	std::uniform_real_distribution<float> positionDist(-citySize, citySize);
	std::uniform_real_distribution<float> canonicalDist;

	for (size_t i = 0; i < numBuildings; i++) {
		auto node = std::unique_ptr<Node>(new Node(mesh, material));

		// set random position
		glm::mat4 model = glm::translate(glm::mat4(1.0f),
			glm::vec3(positionDist(m_rng), positionDist(m_rng), 0.0f)
		);

		// rotate around z with random angle
		model = glm::rotate(model, angleDist(m_rng), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 scale;
		// multiplying uniform distribution will generate beta distribution
		scale.x = canonicalDist(m_rng) * canonicalDist(m_rng) * canonicalDist(m_rng) * canonicalDist(m_rng)
			* (maxBuildingSize - minBuildingSize) + minBuildingSize;
		scale.y = scale.x;
		scale.z = canonicalDist(m_rng) * canonicalDist(m_rng) * canonicalDist(m_rng) * scale.x
			* (maxHeightToWidthRatio - minHeightToWidthRatio) + minHeightToWidthRatio;
		model = glm::scale(model, scale);

		Node* pNode = node.get();
		scene->addNode(std::move(node));
		pNode->setModelMatrix(model);
	}
}
