<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="html"/>

	<xsl:template match="tags">
		<table cellspacing="0" cellpadding="0" border="0" id="selectors-box" class="round-box">
			<tbody>
				<tr>
					<td class="s tl"/>
					<td class="s"/>
					<td class="s tr"/>
				</tr>
				<tr>
					<td class="s"/>
					<td class="c">
						<ul>
								<table>
									<tr>
										<td style="width: 100%">
											<div class="sub-tree-header">Categorías</div>
										</td>
										<xsl:if test="count(tag) &gt; 1">
											<td class="menuIcon">
												<a class="link">
													<xsl:attribute name="onclick">
														advancedSearch( 'advSearch' )
													</xsl:attribute>
													<img style="border:0" src="images/action_adv_search.png" title="Busqueda avanzada" alt="Busqueda avanzada"/>
												</a>
											</td>
										</xsl:if>
									</tr>
								</table>


							<table>
								<xsl:apply-templates select="tag"/>
							</table>
						</ul>
					</td>
					<td class="s"/>
				</tr>
				<tr>
					<td class="s bl"/>
					<td class="s"/>
					<td class="s br"/>
				</tr>
			</tbody>
		</table>
	</xsl:template>

	<xsl:template match="tag">
		<xsl:if test="@readOnly = 0">
			<tr>
				<td class="menuIcon">
					<a class="link">
						<xsl:attribute name="onclick">
							doAction( 'actionCode=' + escape('T2') + '&amp;params=' + escape( 'tagId||#<xsl:value-of select="@id"/>' ), delTagHandler, '')
						</xsl:attribute>
						<img style="border:0" src="images/action_tag_del.png" title="Borrar categoría" alt="Borrar categoría"/>
					</a>
				</td>

				<td class="menuIcon">
					<a class="link">
						<xsl:attribute name="onclick">
							editTag( '<xsl:value-of select="@id"/>', '<xsl:value-of select="@name"/>' )
						</xsl:attribute>
						<img style="border:0" src="images/action_tag_edit.png" title="Editar categoría" alt="Editar categoría"/>
					</a>
				</td>

				<td class="menuName">
					<a class="link">
						<xsl:attribute name="onclick">
							doAction ( 'actionCode=' + escape('A4') + '&amp;params=' + escape( 'tagIds||#<xsl:value-of select="@id"/>|||tagStates||#1|||' ), bodyHandler, 'A' )
						</xsl:attribute>
						<xsl:value-of select="@name"/>
					</a>
				</td>

				<td class="menuName">
					<a class="link">
						<xsl:attribute name="onclick">swapSearch( '<xsl:value-of select="@id"/>' );</xsl:attribute>
						<input type="hidden" value="1"><xsl:attribute name="id">searchtype_<xsl:value-of select="@id"/></xsl:attribute></input>
						<img style="border:0" src="images/action_tag_ignore.gif" title="Ignorar en busqueda avanzada" alt="Ignorar en busqueda avanzada"><xsl:attribute name="id">searchtypeImg_<xsl:value-of select="@id"/></xsl:attribute></img>
					</a>
				</td>
			</tr>
		</xsl:if>
	</xsl:template>
</xsl:transform>