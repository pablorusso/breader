<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="html"/>

	<xsl:template match="tags">
		Haga click sobre una categoria para agregarla:<br/>
		<hr/>
		<xsl:apply-templates select="tag"/>
		<hr/>
		<a class="link" onclick="newTag()">Categoria nueva...</a>
	</xsl:template>					
							
	<xsl:template match="tag">
		<xsl:if test="@readOnly = 0">
			<a class="link">
				<xsl:attribute name="onclick">
					addTag( '<xsl:value-of select="@id"/>' )
				</xsl:attribute>
				
				<xsl:value-of select="@name"/><br/>
			</a>
		</xsl:if>
	</xsl:template>
	
</xsl:transform>