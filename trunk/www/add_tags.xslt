<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:param name="feedId"></xsl:param>
	<xsl:param name="artId"></xsl:param>

	<xsl:output method="html"/>

	<xsl:template match="tags">
		<table cellspacing="0" cellpadding="0" border="0" id="selectors-box2" class="round-box">
			<tr>
				<td class="s tl"/>
				<td class="s"/>
				<td class="s tr"/>
			</tr>
			<tr>
				<td class="s"/>
				<td class="c">
					<table>
						<tr>
							<td class="catName">
								<xsl:if test="count(tag) = 0">
									No hay categorías para agregar.
								</xsl:if>

								<xsl:for-each select="tag">
									<xsl:if test="@readOnly = 0">
										<xsl:if test="position() &gt; 2">
											<xsl:text>, </xsl:text>
										</xsl:if>
										<a class="link">
											<xsl:attribute name="onclick">
												linkTag( '<xsl:value-of select="$feedId"/>','<xsl:value-of select="$artId"/>', '<xsl:value-of select="@id"/>' )
											</xsl:attribute>
											<xsl:value-of select="@name"/>
										</a>
									</xsl:if>
								</xsl:for-each>
							</td>
						</tr>
					</table>
				</td>
				<td class="s"/>
			</tr>
			<tr>
				<td class="s bl"/>
				<td class="s"/>
				<td class="s br"/>
			</tr>
		</table>
	</xsl:template>

</xsl:transform>


